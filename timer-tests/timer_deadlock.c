#include "common.h"

int main() {
	// note: there's lots of stuff in "common.h"

	timer_t ids[20];
	for (unsigned i=0; i<ARRAY_LEN(ids); ++i) {
		ids[i] = create_and_set();
	}

	while(1) {
		pid_t pid = forkx();
		if (pid == 0) {
			// child (tries to) delete timers...
			for (unsigned i=0; i<ARRAY_LEN(ids); ++i) {
				// deleting inherited timers is a POSIX violation, but we don't care here. 
				timer_delete(ids[i]); // should fail if libc adheres to POSIX.
			}
			// ...and exits.
			exit(0);
		} else {
			if (nchildren < 16 || (nchildren & 0xff) == 0) {
				printf("%d: forked child #%d (pid=%d)\n", getpid(), nchildren, pid);
			}
			reap_child();
		}
	}
	return 0;
}

