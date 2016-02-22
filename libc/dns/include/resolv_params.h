#ifndef _RESOLV_PARAMS_H
#define _RESOLV_PARAMS_H

/* The following defines are static */
#define	MAXNS			3	/* max # name servers we'll track */
#define	MAXNSSAMPLES		8	/* max # samples to store per server */

/* The following defines can be overriden through __res_params */
#define SUCCESS_THRESHOLD       0.25    /* if successes / total_samples is less than
                                         * this value, the server is considered failing
                                         */
#define NSSAMPLE_VALIDITY	1800	/* Sample validity in seconds.
                                         * Set to -1 to disable skipping failing
                                         * servers.
                                         */

/* per-netid configuration parameters passed from netd to the resolver */
struct __res_params {
    // threshold of success / total samples below which a server is considered broken
    unsigned char success_threshold; // 0: disable, value / 255 otherwise
    unsigned char min_samples; // min # samples needed for statistics to be considered meaningful
    unsigned short sample_validity; // sample lifetime in s
} __attribute__((__packed__));

#endif // _RESOLV_PARAMS_H
