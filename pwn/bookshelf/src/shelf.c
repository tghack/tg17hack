#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>

/*
 * TODO:
 * 1. overlapping structs
 * 2. trigger UAF
 * 3. exploit UAF
 *
 * example
 * overwrite function pointer with system()
 * argument to original functions must have user-controlled argument
 */

static struct bookshelf *shelf;

struct book {
	char *title;
	char *author;
	uint64_t isbn; /* 10-digit code */
};

struct bookshelf {
	size_t num_books;
	size_t max_books;
	void (*print_book)(struct book *book);
	struct book *books[];
};

static int get_num(void)
{
	char buf[10] = { 0 };

	fgets(buf, sizeof(buf), stdin);
	return strtol(buf, NULL, 10);
}

static void __print_book(struct book *book, bool debug)
{
	FILE *stream;
	const char *template;

	if (debug)
		template = "[+] %s - %s (%d)\n";
	else
		template = "%s - %s (%d)\n";

	stream = debug ? stderr : stdout;
	fprintf(stream, template, book->title, book->author, book->isbn);
}

static void print_book(struct book *book)
{
	__print_book(book, false);
}

static void print_book_debug(struct book *book)
{
	__print_book(book, true);
}

static struct bookshelf *init_shelf(size_t num)
{
	struct bookshelf *b;

	b = malloc(sizeof(struct bookshelf) + (sizeof(struct book *) * num));
	if (!b)
		return NULL;
	b->max_books = num;
	b->num_books = 0;
	b->print_book = print_book;

	return b;
}

static void free_shelf(struct bookshelf *shelf)
{
	free(shelf->books);
	free(shelf);
}

/*
 * TODO: clean up
 */
static void add_book(void)
{
	size_t pos;
	size_t len = 0;
	size_t new_size;
	char *line = NULL;
	struct book *book;

	if (shelf->num_books >= shelf->max_books) {
		/* TODO: check for overflow??? */
		shelf->max_books *= 2;
		new_size = (shelf->max_books * sizeof(struct book *)) +
			   sizeof(struct bookshelf);
		shelf = realloc(shelf, new_size);
		if (!shelf) {
			perror("realloc()");
			exit(EXIT_FAILURE);
		}
	}

	for (pos = 0; shelf->books[pos]; pos++)
		;

	book = malloc(sizeof(struct book));

	printf("Enter author: ");
	if (getline(&line, &len, stdin) == -1) {
		perror("getline()");
		return;
	}
	line[strcspn(line, "\n")] = '\0';
	book->author = strdup(line);

	printf("Enter title: ");
	if (getline(&line, &len, stdin) == -1) {
		perror("getline()");
		return;
	}
	line[strcspn(line, "\n")] = '\0';
	book->title = strdup(line);

	free(line);

	shelf->books[pos] = book;
	++shelf->num_books;
}

static void book_info(void)
{
	int num;
	struct book *book;

	printf("Book number: ");
	num = get_num();
	if (num < 0 || (size_t)num > shelf->num_books) {
		fprintf(stderr, "Invalid index!\n");
		return;
	}

	book = shelf->books[num];
	if (!book) {
		fprintf(stderr, "Book not allocated!\n");
		return;
	}

	shelf->print_book(book);
}

static void delete_book(void)
{
	int num;

	printf("Book number: ");
	num = get_num();
	fprintf(stderr, "WHAT: %d\n", num);
	if (num < 0 || !shelf->books[num]) {
		fprintf(stderr, "Invalid index!\n");
		return;
	}

	free(shelf->books[num]->author);
	free(shelf->books[num]->title);
	free(shelf->books[num]);
	--shelf->num_books;
}

static void enable_debug(void)
{
	shelf->print_book = print_book_debug;
}

static inline int get_yes_no(void)
{
	char buf[10] = { 0 };

	fgets(buf, sizeof(buf), stdin);
	if (buf[0] == 'y' || buf[0] == 'Y')
		return 1;
	return 0;
}

static void edit_book(void)
{
	int num;
	struct book *book;

	printf("Book number: ");
	num = get_num();
	if (num < 0 || !shelf->books[num]) {
		fprintf(stderr, "Invalid index!\n");
		return;
	}

	book = shelf->books[num];

	printf("Change title? (y/n) ");
	if (get_yes_no()) {
		printf("Old title: %s\n", book->title);
		printf("Please enter a new title: ");

		fgets(book->title, strlen(book->title) + 1, stdin);
	}

	printf("Change author? (y/n) ");
	if (get_yes_no()) {
		printf("Old author: %s\n", book->author);
		printf("Please enter a new author: ");
		fgets(book->author, strlen(book->author) + 1, stdin);
	}
}

static inline void print_menu(void)
{
	printf("Super bookshelf manager v4.2\n");
	printf("1. Add book\n");
	printf("2. Print book\n");
	printf("3. Delete book\n");
	printf("4. Edit book\n");
	printf("5. Exit\n");
	printf("> ");
}


static void timer_handler(int signum)
{
	(void)signum;
	fprintf(stderr, "Bye, bye!\n");
	exit(EXIT_SUCCESS);
}

int main(void)
{
	struct sigaction sa = {
		.sa_handler = timer_handler,
		.sa_flags = SA_RESTART
	};

	setvbuf(stdout, NULL, _IONBF, 0);

	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) == -1) {
		perror("sigaction()");
		exit(EXIT_FAILURE);
	}

	alarm(20);

	shelf = init_shelf(2);
	if (!shelf)
		exit(EXIT_FAILURE);

	for (;;) {
		print_menu();
		switch (get_num()) {
		case 1:
			add_book();
			break;
		case 2:
			book_info();
			break;
		case 3:
			delete_book();
			break;
		case 4:
			edit_book();
			break;
		case 5:
			goto out;
		case 42:
			enable_debug();
			break;
		default:
			fprintf(stderr, "Invalid choice!\n");
		}
	}

out:
	free_shelf(shelf);

	return 0;
}
