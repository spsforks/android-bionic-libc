#ifndef _RESOLV_PARAMS_H
#define _RESOLV_PARAMS_H

/* per-netid configuration parameters passed from netd to the resolver */
struct __res_params {
    // threshold of success / total samples below which a server is considered broken
    double success_threshold;
    int min_samples; // min # samples needed for statistics to be considered meaningful
    long sample_validity; // sample lifetime
};

#endif // _RESOLV_PARAMS_H
