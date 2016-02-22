/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <arpa/nameser.h>
#include <string.h>

#include "resolv_stats.h"
#include "private/libc_logging.h"
#include "isc/eventlib.h"

#define DBG 1

/* Calculate the round-trip-time from start time t0 and end time t1. */
int
_res_stats_calculate_rtt(const struct timespec* t1, const struct timespec* t0) {
    // Divide ns by one million to get ms, multiply s by thousand to get ms (obvious)
    long ms0 = t0->tv_sec * 1000 + t0->tv_nsec / 1000000;
    long ms1 = t1->tv_sec * 1000 + t1->tv_nsec / 1000000;
    return (int) (ms1 - ms0);
}

/* Create a sample for calculating server reachability statistics. */
void
_res_stats_set_sample(struct __res_sample* sample, time_t now, int rcode, int rtt)
{
    if (DBG) {
        __libc_format_log(ANDROID_LOG_INFO, "libc", "rcode = %d, sec = %d", rcode, rtt);
    }
    sample->at = now;
    sample->rcode = rcode;
    sample->rtt = rtt;
}

/* Clears all stored samples for the given server. */
void
_res_stats_clear_samples(struct __res_stats* stats)
{
    stats->sample_count = stats->sample_next = 0;
}

/* Aggregates the reachability statistics for the given server based on on the stored samples. */
void
_res_stats_aggregate(struct __res_stats* stats, int* successes, int* errors, int* timeouts,
             int* internal_errors, int* rtt_avg, time_t* last_sample_time)
{
    int s = 0;
    int e = 0;
    int t = 0;
    int ie = 0;
    long rtt_sum = 0;
    time_t last = 0;
    int rtt_count = 0;
    for (int i = 0 ; i < stats->sample_count ; ++i) {
        // Treat everything as an error that the code in send_dg() already considers a
        // rejection by the server, i.e. SERVFAIL, NOTIMP and REFUSED. Assume that NXDOMAIN
        // and NOTAUTH can actually occur for user queries. NOERROR with empty answer
        // section does not lead to a retry by the code in send_dg(), so this is not treated
        // as an error here either. According to information on the internet, FORMERR seems
        // to sometimes be returned by some versions of BIND in response to DNSSEC or EDNS0.
        // Whether to treat such responses as an indication of a broken server is unclear,
        // though. For now treat such responses, as well as unknown codes as errors.
        switch (stats->samples[i].rcode) {
        case NOERROR:
        case NOTAUTH:
        case NXDOMAIN:
            ++s;
            rtt_sum += stats->samples[i].rtt;
            ++rtt_count;
            break;
        case RCODE_TIMEOUT:
            ++t;
            break;
        case RCODE_INTERNAL_ERROR:
            ++ie;
            break;
        case SERVFAIL:
        case NOTIMP:
        case REFUSED:
        default:
            ++e;
            break;
        }
    }
    *successes = s;
    *errors = e;
    *timeouts = t;
    *internal_errors = ie;
    /* If there was at least one successful sample, calculate average RTT. */
    if (rtt_count) {
        *rtt_avg = rtt_sum / rtt_count;
    } else {
        *rtt_avg = -1;
    }
    /* If we had at least one sample, populate last sample time. */
    if (stats->sample_count > 0) {
        if (stats->sample_next > 0) {
            last = stats->samples[stats->sample_next - 1].at;
        } else {
            last = stats->samples[stats->sample_count - 1].at;
        }
    }
    *last_sample_time = last;
}

bool
_res_stats_valid_server(const struct __res_params* params, struct __res_stats* stats) {
    int successes = -1;
    int errors = -1;
    int timeouts = -1;
    int internal_errors = -1;
    int rtt_avg = -1;
    time_t last_sample_time = 0;
    _res_stats_aggregate(stats, &successes, &errors, &timeouts, &internal_errors, &rtt_avg,
            &last_sample_time);
    if (successes >= 0 && errors >= 0 && timeouts >= 0) {
        int total = successes + errors + timeouts;
        if (DBG) {
            __libc_format_log(ANDROID_LOG_DEBUG, "libc", "NS stats: S %d + E %d + T %d + I %d "
                 "= %d, rtt = %d, min_samples = %d\n", successes, errors, timeouts, internal_errors,
                 total, rtt_avg, params->min_samples);
        }
        if (total >= params->min_samples && (errors > 0 || timeouts > 0)) {
            int success_rate = successes * 100 / total;
            if (DBG) {
                __libc_format_log(ANDROID_LOG_DEBUG, "libc", "success rate %d%%\n", success_rate);
            }
            if (success_rate < params->success_threshold) {
                time_t now = evNowTime().tv_sec;
                if (now - last_sample_time > params->sample_validity) {
                    if (DBG) {
                        __libc_format_log(ANDROID_LOG_INFO, "libc",
                            "samples stale, retrying server\n");
                    }
                    _res_stats_clear_samples(stats);
                } else {
                    if (DBG) {
                        __libc_format_log(ANDROID_LOG_INFO, "libc",
                            "too many resolution errors, ignoring server\n");
                    }
                    return 0;
                }
            }
        }
    }
    return 1;
}

void
_res_stats_get_valid_servers(const struct __res_params* params, struct __res_stats stats[MAXNS],
        int nscount, bool valid_servers[MAXNS]) {
    unsigned valid_servers_found = 0;
    for (int ns = 0; ns < nscount; ns++) {
        bool valid = _res_stats_valid_server(params, &stats[ns]);
        if (valid) ++valid_servers_found;
        valid_servers[ns] = valid;
    }
    // If there are no valid servers, consider all of them valid
    if (valid_servers_found == 0) {
        memset(valid_servers, 1, nscount);
    }
}
