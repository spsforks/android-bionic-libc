#ifndef _RESOLV_PARAMS_H
#define _RESOLV_PARAMS_H

/* per-netid configuration parameters passed from netd to the resolver */
struct __res_params {
    double success_threshold; // threshold of success / total samples below which a server is considered broken
    int min_samples; // min # samples needed before servers can be considered broken
    long sample_validity; // time in s that a sample is considered valid
};

#endif // _RESOLV_PARAMS_H
