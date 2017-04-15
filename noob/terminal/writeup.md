# Terminal Writeup

### Level 1
1. List all the files. See anything interesting?

`$ ls` to list all files and directories. The flag is `TG17{the_first_step}` as seen in the list.

2. View the contents of `flag.txt`.

`$ cat flag.txt` to output the contents, which is the flag `TG17{show_me_your_contents}`. 

3. What's inside the directory?

`$ ls flag_here` to check the contents of the directory.

`$ cd flag_here` to change to directory containing the file flag.txt. 

`$ cat flag.txt` output the contents of file flag.txt. The flag is `TG17{linux_so_fun}` as the output says.


### Level 2
1. There's a hidden file in the current directory, can you find it?

`$ ls -a` shows all hidden files, as specified in the top of the option list in the man pages. 

`$ cat .flag.txt` to output flag. The flag is `TG17{s0_hidden}`. 

2. The next file isn't as easy to find! Maybe you have to look through some hidden directories as well.

`$ ls -a` shows hidden directories as well, an show the directory `.hidden`. After some exploring, we see that an option is as follows:

`$ cd .hidden/more_hidden/super_hidden/` to enter a directory, which seems to be empty. 

`$ ls -a` to find another `.flag.txt`, and then try `$ cat .flag.txt`, which gives us the flag `TG17{g0_away_please}`.

3. Even harder this time! Maybe you need an overview of the file structure.
`$ tree`

### Level 3
1. There's a weird username on the system. Can you find it? Submit as TG17{\<username\>}.

`$ cat etc/passwd` output contents of the passwd-file, containing information about users on the system. There is a suspicious user, which is the content of the flag, `TG17{user_flag_y000}`.

2. There's a weird group name on the system. Can you find it? Submit as TG17{\<groupname\>}.

`$ cat etc/group` output the contents of the file group, containing information about the groups on the system. The suspicious group `this_is_the_group_fl4g` is the contents of the flag.

3. There's something weird in the kernel log. *Hint:* use grep.

`$ dmesg | grep TG17`. The command `dmesg` output the kernel module, and pipe `|` use grep on the output, looking for the text `TG17`. The flag is `TG17{i_wrote_a_kernel_module_for_this?!}`.

### Level 4
1. Can you extract the contents of the gz archive?

`$ tar -xf flag.tar.gz`. As the man page says, -x is used to extract the file. -f is added to tell the command it as a file. Use cat to output the flag from the file `flag.txt`, getting `TG17{c0mpressi0n_101}`.

2. Can you extract the contents of the zip archive?

`$ unzip flag.zip` to extract the file, use cat to get the flag, `TG17{unzip_me_please}`.

3. Can you extract the contents of the rar archive?

`$ rar x flag.rar` to extract the file, cat to output `TG17{we_d0nt_need_no_winrar}`.


### Level 5
1. Try to sort `sort.txt`!

`$ sort sort.txt` sort the lines of the file, so that the flag appears on the end. The flag is `TG17{Let's_s0rt_this_out}`.

2. Can you find the only unique word in `uniq.txt`?

`$ uniq -u uniq.txt` output the only uniqe line containing the flag. Has written in the man page, -u only prints the uniqe lines. The flag is `TG17{I'm_such_a_unique_snowflake}`.

3. Look for the flag pattern in `grep.txt`.

`$ grep TG17 grep.txt` output the flag, `TG17{grepping_all_day}`.


However, all the tasks in level 5 could be solved by `$ grep TG17 [filename]`
