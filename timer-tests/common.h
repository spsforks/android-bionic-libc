#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_LEN(x) (sizeof((x)) / sizeof((x)[0]))

static int depth      = 0;
static int nchildren  = 0;
static int my_childno = 0;

static void timer_callback(union sigval sv) {
	sv = sv; // Yes, it is unused. Thank you, gcc.
	// do nothing.
}

static void info(const char* msg) {
	fprintf(stderr, "%d: fork depth=%d, child number %d: ", getpid(), depth, my_childno);
	perror(msg);
}

static int forkx() {
	pid_t ret = fork();
	nchildren++;
	if (ret == 0) {
		my_childno = nchildren;
		nchildren = 0;
		depth++;
	} else if (ret < 0) {
		info("Failed to fork");
		exit(20);
	}
	return ret;
}

static pid_t reap_child() {
	int status;
	pid_t pid = wait(&status);
	if (pid < 0) {
		perror("wait() failed");
		exit(21);
	}
	if (!WIFEXITED(status)) {
		fprintf(stderr, "child %d exited in an unexpected way: 0x%x\n", pid, status);
		exit(22);
	}
	status = WEXITSTATUS(status);
	if (status != 0) {
		fprintf(stderr, "child %d exited with non-zero code: %d\n", pid, status);
		exit(status);
	}
	return pid;
}

static timer_t create_and_set() {
	struct sigevent ev;
	memset(&ev, 0, sizeof(ev));
	ev.sigev_notify = SIGEV_THREAD;
	ev.sigev_notify_function = timer_callback;
	ev.sigev_notify_attributes = NULL;
	timer_t timerid;
	
	if (timer_create(CLOCK_MONOTONIC, &ev, &timerid) != 0) {
		info("Failed to create timer");
		exit(23);
	}
	
	struct itimerspec spec;
	spec.it_interval.tv_sec  = 1;
	spec.it_interval.tv_nsec = 0;
	spec.it_value = spec.it_interval;
	if (timer_settime(timerid, 0, &spec, NULL) != 0) {
		info("Failed to set timer");
		exit(24);
	}
	
	printf("%d: Created timer, id is 0x%08x\n", getpid(), timerid);

	return timerid;
}

