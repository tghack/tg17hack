#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

static char flag[] = "\x24\x26\x42\x44\x0c\x5e\x1d\x3b\x01\x3a\x12\x54\x00\x08\x1a\x31\x5b\x1c\x06\x2b\x07\x2d\x02\x1c\x04\x12\x30\x06\x0c\x3a\x3b\x1f\x03\x1c\x1f\x1c";
static char password[] = "\x9e\x8e\x83\x82\x85\xc3\x86\x91\x85\xa8\x8b\xc9\xa5\x9d\x89\x93\xd2\x91\x6f\x5e\x60\x71\x71\x71\x63\x61\x67\x7b\x69\x6e\x53\x7d\x62\x7c";

static __attribute__((noinline)) int check_pw(const char *pw)
{
	if (strlen(pw) != strlen(password))
		return 0;

	uint32_t start = 0xc0ffee;
	int lim = (int)strlen(pw);
	char buf[sizeof(password)] = { 0 };

	for (int i = 0; i < lim; i++)
		buf[i] = ((unsigned char)pw[i] ^ (start + i)) & 0xff;

	for (int i = 0; i < lim; i++) {
		if (buf[i] != password[i])
			return 0;
	}

	return 1;
}

static __attribute__((noinline)) void print_flag(const char *pw)
{
	char flagbuf[sizeof(flag)] = { 0 };
	int lim = (int)(sizeof(flag) - 1);

	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
		for (int i = 0; i < lim; i++)
			flag[i] += i * 3;
	}

	for (int i = 0; i < lim; i++)
		flagbuf[i] = (unsigned char)flag[i] ^ (unsigned char)pw[i % strlen(pw)];

	printf("flag: %s\n", flagbuf);
}

int main(void)
{
	char buf[256] = { 0 };

	printf("You are fantastic\n");
	printf("Password please: ");
	if (!fgets(buf, sizeof(buf), stdin)) {
		perror("fgets()");
		exit(EXIT_FAILURE);
	}

	buf[strcspn(buf, "\n")] = '\0';

	if (check_pw(buf)) {
		printf("yay\n");
		print_flag(buf);
	} else {
		printf("nope :(\n");
	}

	return 0;
}
