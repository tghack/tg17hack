#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/user.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#define __noreturn __attribute__((noreturn))

#define pr_info(fmt, ...) do { fprintf(stderr, "[+] " fmt, ## __VA_ARGS__); } while(0)

static int debug = 1;
static bool first = 1;
static bool entering = 1;

static int proc_syscall(pid_t pid)
{
	long ret;

	ret = ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	if (ret == -1) {
		perror("ptrace syscall");
		return -1;
	}

	return 0;
}

static __noreturn void perr_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

static inline unsigned long proc_read_word(pid_t pid, void *addr)
{
	return ptrace(PTRACE_PEEKTEXT, pid, addr, NULL);
}

static int proc_get_regs(pid_t pid, struct user_regs_struct *regs)
{
	long ret;

	ret = ptrace(PTRACE_GETREGS, pid, NULL, regs);
	if (ret == -1) {
		perror("ptrace getregs");
		return -1;
	}

	return 0;
}

static pid_t proc_start(const char *filename)
{
	int status;
	pid_t pid;
	char *args[2] = { NULL };

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (!pid) {
		/*
		 * tell tracer to trace us and send sigstop, so that the
		 * tracer has time to trace everything we do
		 */
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		kill(getpid(), SIGSTOP);
		args[0] = (char *)filename;
		execv(filename, args);
		exit(EXIT_FAILURE);
	}

	while (waitpid(pid, &status, WSTOPPED) < 0) {
		if (errno == EINTR)
			continue;
		perr_exit("waitpid");
	}

	if (!WIFSTOPPED(status) || WSTOPSIG(status) != SIGSTOP)
		perr_exit("Unexpected wait status");
	
	return pid;
}

static int proc_step(pid_t pid)
{
	long ret;

	ret = ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
	if (ret == -1) {
		perror("ptrace singlestep");
		return - 1;
	}

	return 0;
}

static int proc_step_and_wait(pid_t pid)
{
	int ret;
	siginfo_t info;

	ret = proc_step(pid);
	if (ret)
		return -1;

	waitid(P_PID, pid, &info, WSTOPPED);
	return 0;
}

static int proc_read_bytes(pid_t pid, char *address, void *buf, size_t num_bytes)
{
	char *bufp = buf;
	size_t lim = num_bytes / sizeof(long);
	if (num_bytes % sizeof(long))
		++lim;

	fprintf(stderr, "num_bytes / lim: %zu / %zu\n", num_bytes, lim);
	for (size_t i = 0; i < lim; i++) {
		*(unsigned long *)(bufp + i * sizeof(long)) = proc_read_word(pid, address + i * sizeof(long));
	}

	return 0;
}

static void find_elf_header(pid_t pid)
{
	struct user_regs_struct regs;
	if (proc_get_regs(pid, &regs))
		return;

	unsigned long long prev_rip;
	prev_rip = regs.rip;
	unsigned long long mask = 0xffffffff00000000;

	for (;;) {
		if (proc_get_regs(pid, &regs))
			return;

		if ((regs.rip & mask) != (prev_rip & mask)) {
			prev_rip = regs.rip;
			break;
		}

		prev_rip = regs.rip;
		proc_step_and_wait(pid);
	}

	/* align on page size */
	unsigned long long addr;
	addr = ((prev_rip + 4096 - 1) / 4096) * 4096;
	const char *elf_head = "\x7f\x45\x4c\x46";

	for (;;) {
		unsigned long val = proc_read_word(pid, (void *)addr);
		if (memmem(&val, sizeof(unsigned long), elf_head, 4))
			break;

		addr -= 8;
	}

#if 0
	/* get XORed password */
	size_t xor_offset = 0x202018;
	char buf[64] = { 0 };
	proc_read_bytes(pid, (char *)addr + xor_offset, buf, sizeof(buf));
	FILE *fp = fopen("password.bin", "wb+");
	assert(fp);
	fwrite(buf, sizeof(buf), 1, fp);
#else
	/* we should have a valid address now! */
	Elf64_Ehdr head;
	proc_read_bytes(pid, (char *)addr, &head, sizeof(head));

	/* calculate size */
	size_t file_size = head.e_shoff + (head.e_shentsize * head.e_shnum);
	char *buf = calloc(1, file_size + 8);
	assert(buf);
	proc_read_bytes(pid, (char *)addr, buf, file_size);

	FILE *fp = fopen("test.bin", "wb+");
	assert(fp);
	fwrite(buf, file_size, 1, fp);

	free(buf);
#endif
	fclose(fp);

	pr_info("Job's done!\n");

	exit(EXIT_SUCCESS);
}

static int handle_syscall_exit(pid_t pid)
{
	if (first) {
		fprintf(stderr, "[+] process done with execve!\n");
		first = false;
		find_elf_header(pid);
	}

	return 0;
}

static int handle_syscall_enter(pid_t pid)
{
	long ret;
	struct user_regs_struct regs;
	entering = false;

	ret = proc_get_regs(pid, &regs);
	if (ret)
		return -1;

	return 0;
}

static inline int handle_syscall(pid_t pid)
{
	return entering ? handle_syscall_enter(pid) : handle_syscall_exit(pid);
}

static int trace(pid_t pid)
{
	pid_t retp;
	int status;
	unsigned int event;

	/* TODO: can this make us wait indefinitely? */
	retp = waitpid(pid, &status, 0);

	if (retp == -1) {
		if (errno == EINTR) {
			if (debug)
				fprintf(stderr, "%s: interrupted waitpid\n", __func__);
			return 1;
		}

		if (errno == ECHILD) {
			if (debug)
				fprintf(stderr, "%s: child died\n", __func__);
			return 0;
		}
	}

	event = (unsigned int)status >> 16;

	/* TODO: print signal information */
	if (WIFSIGNALED(status)) {
		if (debug)
			fprintf(stderr, "[d] %s: process was signalled\n", __func__);
		return 1;
	}

	if (WIFEXITED(status)) {
		if (debug)
			fprintf(stderr, "[d] %s: process exited\n", __func__);
		return 0;
	}

	if (!WIFSTOPPED(status)) {
		if (debug)
			fprintf(stderr, "[d] %s: not stopped? wtf\n", __func__);
		return 1;
	}

	int sig = WSTOPSIG(status);

	if (event != 0)
		goto restart_zero;

	if (sig != (SIGTRAP | 0x80)) {
		if (debug) {
			fprintf(stderr, "[d] %s: Process was stopped: %d (%s)\n",
					__func__, sig, strsignal(sig));
		}

		/* TODO: check signal information */
		if (sig == SIGSEGV) {
			fprintf(stderr, "[-] Caught SIGSEGV\n");
			return 0;
		} else if (sig == SIGBUS) {
			fprintf(stderr, "[-] Caught SIGBUS\n");
			return 0;
		} else if (sig == SIGILL) {
			fprintf(stderr, "[-] Caught SIGILL; what the fuck are you doing?\n");
			return 0;
		} else if (sig == SIGPIPE) {
			/*
			 * this can happen if the program tries to write to
			 * a socket that isn't connected
			 */
			fprintf(stderr, "[-] Caught SIGPIPE. socket not connected?\n");
			return 0;
		}
		goto restart_proc;
	}

	/* enter_syscall_stop or exit_syscall_stop */
	int ret = handle_syscall(pid);
	if (ret)
		return 0;

restart_zero:
	sig = 0;

restart_proc:
	if (ptrace(PTRACE_SYSCALL, pid, NULL, sig) < 0) {
		fprintf(stderr, "%s: noooo :(\n", __func__);
		return 0;
	}

	return 1;
}


int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	long trace_options = PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC;
	pid_t pid = proc_start(argv[1]);

	long ret = ptrace(PTRACE_SETOPTIONS, pid, NULL, trace_options);
	if (ret == -1) {
		perror("ptrace setoptions");
		goto fail;
	}

	if (proc_syscall(pid))
		return -1;

	pr_info("Initialization done! Entering trace loop\n");

	while (trace(pid))
		;

	return 0;
fail:
	exit(EXIT_FAILURE);
}
