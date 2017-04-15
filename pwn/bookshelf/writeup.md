# Bookshelf writeup

The bookshelf structure is like this:
```C
struct bookshelf {
        size_t num_books;
        size_t max_books;
        void (*print_book)(struct book *book);
        struct book *books[];
};
```

We can see that `realloc()` is used to increase the size of the whole bookshelf when we insert more books than the shelf can hold (controlled by `max_books`).

When books are deleted, the pointers aren't set to `NULL`, so we have a use-after-free bug.

When adding a book, `strdup()` is used to allocate room for our title and author strings.

Knowing this, we can try to allocate a title string long enough to hold the reallocated bookshelf, and then freeing the book object. This will give us control over the book pointers inside the shelf when it's reallocated.

The plan is then to set up one of the books (number 4) so that we can overwrite the GOT entry of `strtol`. We find the correct address:
```
$ readelf --relocs shelf | grep strtol
000000601bd8  000b00000007 R_X86_64_JUMP_SLO 0000000000000000 strtol@GLIBC_2.2.5 + 0
$ gdb ./shelf -ex "b main" -ex "r"
[...]
gdb-peda$ find 0x000000601bd8
Searching for '0x000000601bd8' in: None ranges
Found 1 results, display max 1 items:
uaf : 0x400720 --> 0x601bd8 --> 0x4008c6 (<strtol@plt+6>:	push   0x9)
```

The program will treat 0x400720 as a `book *`, and we can change the title, which is the first pointer. This makes it possible to change the GOT entry of `strtol()`.

Run [this script](src/book.py) to pwn. The script does the following:

1. allocate a new book with the address above in index 3 (fourth book)
2. delete book
3. add new book * 2
3. add new book -> trigger realloc
4. edit book -> trigger UAF
5. edit_book leaks the address of `strtol()`, calculate libc base and `system()`
6. overwrite with `system()`
7. send `/bin/sh` and we got a shell!
