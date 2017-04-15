#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libc_base.h"

#define MAX_NOTES 5
static char *notes[MAX_NOTES];
static size_t num_notes;

static const char flag[] = "TG17{please_fix_my_leaky_bucket}";

static inline void print_menu(void)
{
	printf("Heaps of fun 0.1\n");
	printf("1. Add note\n");
	printf("2. Delete note\n");
	printf("3. Print note\n");
	printf("4. Get flag\n");
	printf("5. Exit\n");
	printf("$ ");
}

static inline void get_flag(void)
{
	uint64_t addr;
	char buf[64] = { 0 };

	printf("Give me the libc base address: ");
	if (!fgets(buf, sizeof(buf), stdin))
		return;

	addr = strtol(buf, NULL, 16);
	printf("You entered: 0x%lx\n", addr);
	if (addr == get_libc_addr())
		printf("Yay! Here's the flag: %s\n", flag);
	else
		fprintf(stderr, "Nope :( Try again!\n");
}

static inline void add_note(void)
{
	if (num_notes >= MAX_NOTES) {
		fprintf(stderr, "No more room for notes!\n");
		return;
	}

	char buf[15] = { 0 };
	int num;
	printf("Please enter note size: ");
	if (!fgets(buf, sizeof(buf), stdin))
		return;

	num = atoi(buf);
	if (num > 512) {
		fprintf(stderr, "Too large!\n");
		return;
	}

	char *ptr = malloc(num);
	notes[num_notes++] = ptr;

	printf("Enter note contents: ");
	if (!fgets(ptr, num, stdin)) {
		free(ptr);
		--num_notes;
		return;
	}

	printf("Note added!\n");
}

static inline int note_number(void)
{
	int num;
	char buf[15] = { 0 };

	printf("Note number: ");
	if (!fgets(buf, sizeof(buf), stdin))
		return -1;

	num = atoi(buf);
	if (num >= MAX_NOTES) {
		fprintf(stderr, "Number too large!\n");
		return -1;
	}

	return num;
}

static inline void del_note(void)
{
	int num;
	char buf[15] = { 0 };

	num = note_number();
	if (num == -1)
		return;

	free(notes[num]);
	printf("Deleted note %d\n", num);
}

static inline void print_note(void)
{
	int num;

	num = note_number();
	if (num == -1)
		return;

	if (!notes[num])
		return;
	printf("%s", notes[num]);
}

static inline void edit_note(void)
{
	int num;
	char buf[256] = { 0 };

	num = note_number();
	if (num == -1)
		return;

	if (!notes[num])
		return;

	printf("New contents: ");
	if (!fgets(buf, sizeof(buf), stdin))
		return;

	strcpy(notes[num], buf);
}

int main(void)
{
	char buf[512];
	setvbuf(stdout, NULL, _IONBF, 0);

	for (;;) {
		print_menu();
		if (!fgets(buf, sizeof(buf), stdin))
			break;

		int num = atoi(buf);
		switch (num) {
		case 1:
			add_note();
			break;
		case 2:
			del_note();
			break;
		case 3:
			print_note();
			break;
		case 4:
			get_flag();
			break;
		case 5:
			printf("Bye!\n");
			return 0;
		default:
			printf("Invalid choice\n");
			break;
		}
	}

	return 0;
}
