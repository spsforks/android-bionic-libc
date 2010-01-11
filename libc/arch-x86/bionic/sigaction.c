#include <signal.h>

extern int __sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    return __sigaction(signum, act, oldact);
}
