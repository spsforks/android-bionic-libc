#include "common.h"

int main() {
	// note: there's lots of stuff in "common.h"

	while(1) {
		timer_t timerid = create_and_set();
		timer_delete(timerid);
		pid_t pid = forkx();
		if (pid == 0) {
			printf("%d: I am child #%d\n", getpid(), depth);
			// child loops around to fork a grandchild...
		} else {
			printf("%d: forked pid=%d\n", getpid(), pid);
			if (depth == 0) {
				// first process: sleep forever (to keep adb shell up).
				reap_child();
				while (1) {
					sleep(10);
				}
			} else {
				// parent exits
				exit(0);
			}
		}
	}
	return 0;
}

