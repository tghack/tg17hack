#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static void __attribute__((noreturn)) alarm_handler(int sig)
{
	(void)sig;
	printf("Bye!\n");
	exit(EXIT_SUCCESS);
}

int main(void)
{
	char name[64] = { 0 };

	signal(SIGALRM, alarm_handler);
	alarm(15);

	setvbuf(stdout, NULL, _IONBF, 0);

	printf("What's your name? ");
	if (!fgets(name, sizeof(name), stdin))
		exit(EXIT_FAILURE);

	name[strcspn(name, "\n")] = '\0';

	char cmd[512] = { 0 };
	snprintf(cmd, sizeof(cmd) - 1, "echo \"Hello %s, here's the current time: \" && date", name);

	printf("\n");
	return system(cmd);
}
