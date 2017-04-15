#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	/* Force order of variables on the stack */
	struct {
		char buf[64];
		uint32_t num;
	} lol;
	lol.num = 0x1337;

	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Change to 0xc0ffee\n");
	printf("old value: 0x%08x\n", lol.num);

	printf("Give me some input: ");
	fgets(lol.buf, 128, stdin);
	printf("new value: 0x%08x\n", lol.num);

	if (lol.num == 0xc0ffee)
		printf("Yay! The token is: %s\n", TOKEN);
	else
		printf("Try again!\n");

	exit(EXIT_SUCCESS);
}
