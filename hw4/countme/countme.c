#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

void errquit(const char *msg) {
	perror(msg);
	exit(-1);
}

int main(int argc, char *argv[]) {
	pid_t child;
	if(argc < 2) {
		fprintf(stderr, "usage: %s program\n", argv[0]);
		return -1;
	}
	if ((child = fork()) < 0) errquit("fork");
	if (child == 0) {
		if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) errquit("ptrace");
		execvp(argv[1], argv+1);
		errquit("execvp");
	} else {
		long long counter = 0LL;
		int status;
		if (waitpid(child, &status, 0) < 0) errquit("waitpid");
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL);
		while (WIFSTOPPED(status)) {
			counter++;
			if (ptrace(PTRACE_SINGLESTEP, child, 0, 0) < 0) errquit("ptrace@parent");
			if (waitpid(child, &status, 0) < 0) errquit("wait");
		}
			
		fprintf(stderr, "## %lld instructions exected\n", counter);
	}

	return 0;
}

