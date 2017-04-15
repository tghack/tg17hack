### Writeup

This is a simple program that uses `fopen` to open a file named "make\_me"
in mode `"r+"`, which is read and write. This mode fails if the file does not
exist. 

Running `strings` on the file gives the string "make\_me", which in itself
is a hint thats not super easy to understand. `strings` is a command line
program that prints all ascii text strings in a file, you can read more
about strings [here](https://linux.die.net/man/1/strings) and about the
definition of a "ascii text string" [here](https://www.tutorialspoint.com/cprogramming/c_strings.htm)

The easiest way to solve this is to run `strace` on the program to see what
it does. Then you can se that it tries to open the file with the specified
name (strace tracks system calls done by a program, and in this case the
program tries to do fopen). You can read more about strace [here](http://www.tutorialspoint.com/unix_commands/strace.htm).

This can also be discovered through reverse engineering of the binary file
to discover how it operates, although this is way more complicated than
what is needed to solve this task.

To solve the task just create the file as specified in the same directory
as the binary, and then run the program, and the flag will be written to
the file.

Flag is TG17{abcdefghijklmnopqrs}
