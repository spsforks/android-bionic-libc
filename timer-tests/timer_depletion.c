#include "common.h"

static int do_del       = 0;
static int del_expected = 0;

static void maybe_delete(timer_t timerid) {
	if (do_del) {
		int ret = timer_delete(timerid);
		if (ret != del_expected) {
			fprintf(stderr, "%d: got %d, but expected %d\n", getpid(), ret, del_expected);
			info("Unexpected return value from timer_delete");
			exit(2);
		}
	}
}

static void parse_args(int argc, char** argv) {
	for (int i=1; i<argc; ++i) {
		if (strcmp(argv[i], "faildel") == 0) {
			do_del = 1;
			del_expected = -1;
		} else if (strcmp(argv[i], "del") == 0) {
			do_del = 1;
		} else {
			fprintf(stderr, "Illegal argument \"%s\"\n", argv[1]);
			exit(1);
		}
	}
}

int main(int argc, char** argv) {
	// note: there's lots of stuff in "common.h"
	parse_args(argc, argv);

	while(1) {
		timer_t parent_timer = create_and_set();
		pid_t pid = forkx();
		if (pid == 0) {
			printf("%d: I am child #%d\n", getpid(), depth);
			// deleting is a POSIX violation -- should not use inherited timers.
			maybe_delete(parent_timer);
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

