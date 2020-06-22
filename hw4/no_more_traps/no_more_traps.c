#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

void dump_code(long addr, long code) {
	fprintf(stderr, "## %lx: code = %02x %02x %02x %02x %02x %02x %02x %02x\n",
		addr,
		((unsigned char *) (&code))[0],
		((unsigned char *) (&code))[1],
		((unsigned char *) (&code))[2],
		((unsigned char *) (&code))[3],
		((unsigned char *) (&code))[4],
		((unsigned char *) (&code))[5],
		((unsigned char *) (&code))[6],
		((unsigned char *) (&code))[7]);
}

void errquit(const char *msg) {
	perror(msg);
	exit(-1);
}

int readfile (unsigned long *opcode) {
	FILE *fp;
	if((fp=fopen("no_more_traps.txt","r"))==NULL){
		printf("open file error!!\n");
		exit(0);
	}
	int i = 0;
	while (fscanf(fp, "%02lx", &opcode[i]) != EOF)
		i++;
	fclose(fp);
	return 1;
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "usage: %s program\n", argv[0]);
		return -1;
	}
	unsigned long *opcode = malloc(7000*sizeof(unsigned long));
	if(!readfile(opcode)) errquit("readfile");
	pid_t child;
	if ((child = fork()) < 0) errquit("fork");
	if (child == 0) {
		if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) errquit("ptrace");
		execvp(argv[1], argv+1);
		errquit("execvp");
	} else {
		// long long counter = 0LL;
		unsigned long code, target, replaced_code;
		int index = 0;
		int status;
		if (waitpid(child, &status, 0) < 0) errquit("waitpid");
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL);
		ptrace(PTRACE_CONT, child, 0, 0);
		while (waitpid(child, &status, 0) > 0) {
			struct user_regs_struct regs;
			if(!WIFSTOPPED(status)) continue;
			if(ptrace(PTRACE_GETREGS, child, 0, &regs) != 0)
				errquit("ptrace(GETREGS)");
			target = regs.rip-1;
			code = ptrace(PTRACE_PEEKTEXT, child, target, 0);
			replaced_code = (code & 0xffffffffffffff00) | *(opcode+index);
			// dump_code(target, code);
			if(ptrace(PTRACE_POKETEXT, child, target, replaced_code) != 0)
				errquit("ptrace(POKETEXT)");
			// dump_code(target, replaced_code);
			index++;
			// set registers 
			regs.rip = regs.rip-1;
			if(ptrace(PTRACE_SETREGS, child, 0, &regs) != 0) errquit("ptrace(SETREGS)");
			ptrace(PTRACE_CONT, child, 0, 0);
		}
	}
	return 0;
}