# Tips & Tricks
This page is meant to help you on your journey of becoming a 1337 hacker (in CTFs). We will tell you about a few tools that will help you on your way. A part of the hacker training is to get your black belt in google-foo. You need to train your google-foo skills, so we can not provide you with much information on how to use the tools! As all hackers know, the learning curve in hacking is tremendously steep, but when your hacker and google skills arrive at a certain level, the thirst for knowledge is hard to stop and you suddenly obtain your black belt in google-foo.


As a little starting kit for your journey, we give you a little tutorial on how to use the Linux terminal. Those of you who have a little experience with Linux terminals may skip this tutorial and find the list of tools further down the page.


### Beginner?
Are you a beginner? Don't be afraid! We give you this gift to start your hacker journey: [Starting Kit](#starterkit) 

---


## Tools
#### General tools
* Google, google and more google: Searching for information and learning new things by doing so, might be the most powerful tool for hackers! And remember, you need to train hard to get your google-foo black belt! [Link](http://bfy.tw/5g)
* IDA: A nice tool for reverse engineering. [Link to IDA download](https://www.hex-rays.com/products/ida/support/download_freeware.shtml) (Reverse engineering)
* By googleing: Web pages with converting programs, e.g. hex to ascii. Calculators and decoders for different ciphers within cryptography. And a lot more!!!
* [Sagemath](http://www.sagemath.org/) (crypto)
* [Burp Suite](https://portswigger.net/burp/) (web)
* [Online disassembler](https://www.onlinedisassembler.com/static/home/)
* DevTools in web browser: When in a website, open with **_Ctrl+Shift+I_** or **_f12_**. (web)
* [Fiddler](http://www.telerik.com/fiddler) (web)
* Wireshark and tshark


#### Python
* Python is a very easy, yet powerful scripting language. It is very relevant for CTFs. Python probably can be used for the categories of CTFs.
* [Codecademy](https://www.codecademy.com/): Nice page to learn python. 
* pwn-tools: Add **_from pwn import \*_** at the top of the file to enable pwn-tools in your python script. The python library is especially good for connecting to remote servers, e.g. the function **_remote()_**. (pwn, programming)


#### Terminal tools
* gdb: GNU Debugger 
* gdb-peda: Python plugin for gdb. [Link to github](https://github.com/longld/peda)
* man pages: Manual pages for functions. Write **_man_** in front of any terminal command or C function. E.g. `$ man ls` or `$ man strings`. You will probably not get your black belt without any man page XP. 
* `strings`: Use the man page.
* `file`: Use the man page.
* `ltrace`: Tracing library calls. Use the man page.
* `strace`: Tracing syscalls. Use the man page. 


#### Nice puzzles for learning more hacking 
* [Overthewire](http://overthewire.org/wargames/) is a very nice website for different puzzles. The category **bandit** has terminal puzzles and **narnia** contains exploit intro puzzles.
* [pwnable.kr](http://pwnable.kr/) contains exploit puzzles.
* [cryptopals crypto challenges](https://cryptopals.com/sets/1) contains, as its name says, crypto challenges. 
* If someone know about some nice pages for learning more hacking, please tell us!


---


## Contacting tghack crew

**IRC:** tghack@efnet

**UiO Stand:** Don't be afraid to come to the University of Oslo stand asking questions. We are more than happy to talk with you guys!

<a name="starterkit"></a>

-----


# N00b Starter Kit
As a little starting kit for your hacker journey, we give you a little tutorial on how to use the Linux terminal. The Linux terminal is an invaluable tool, used by virtually every security professional. Using a terminal can be hard sometimes, especially if you're used to point-and-click. The terminal enables us to run commands, to perform both simple and very complex tasks. Some of which are impossible when using a GUI. 


Of all the ways to get a terminal, we recommend either using PuTTY or downloading a program for hosting virtual machine, such as VirtualBox. A virtual machine is exactly what it sound like, another machine running on your machine. Virtual machines are referred to as a VM, and you may install whatever operating system you would like on any of your VMs. Ubuntu 16.04 is a nice operating system to start with. HOWEVER, we do not recommend installing VirtualBox and make a VM just for this competition(!!). If you are interested in programming and curious about Linux, VMs may be better than PuTTY. 


We provide a tutorial for downloading and using PuTTY. This tutorial helps you through the essentials for solving some of the task in the competition, and will hopefully get you started on your way to become a terminal wizard.


[Follow this link to the tutorial](#tutorial)


Do you want to learn even more terminal commands? Check out this really nice website: [Linux Command website](http://linuxcommand.org).


### How to get even more hacker skills
As you now have been through the tutorial, you must be aware that the learning curve of becoming a hacker is very steep and your most powerful tool is searching for every little thing that is unclear. 


If you liked the tutorial and want to learn more about Linux and the terminal, there are some things that might be quite nice to know about, such as how to install stuff in the terminal, and what text editors to use. The two remaining sections are meant for those who want to give the terminal a try, and use a Linux operating system. 


### How to: install stuff in the terminal
This short guide is for the operating system Ubuntu, and might therefore not work in some other Linux OS. In order to install software on Ubuntu, write `sudo apt-get install [name-of-software]` in the terminal. E.g. you may install Vim by writing `sudo apt-get install vim`. Since almost every installation needs root-permissions, you must use the word **_sudo_**, which then asks for the root password. Remember to read some of the last sentences to make sure that the software indeed was installed. Errors occurs if not installed properly.


### Text editors
* **Vim:** A terminal text editor. We LOVE vim, but it might kind of hard to learn in the very beginning. We're rooting for you! Hint: if you need to exit and don't care about saving, type **_[esc]_**, then **_:q!_** and then **_[enter]_** to run your command. If you have not done any changes, **_:q_** is all you need to type. To save **_[esc] :w_**, to save and quit **_[esc] :wq_**. Google to learn more!
* **atom**
* **sublime**
* This hurt our hearts, but: Emacs... :'( Please don't. 

<a name="tutorial"></a>

---


# Tutorial

If you are using mac OS or Linux, you can use the built-in terminal to test commands etc. If you are using Windows, we recommend either running Kali Linux in a virtual machine, or logging in to our server using ssh. If you have any trouble getting a terminal to work, don't hesitate to ask at the UiO stand for help, or on IRC.


When reading the tutorial, you can test the commands on our server by logging in as the user tutorial. The password is TG2017. We highly recommend you to log in and play around in the terminal while reading the tutorial!


### Connecting to the server
This is optional. If you're already familiar with the terminal, and have a terminal to play with, you can skip this step. If you're using Windows, download PuTTY from [here](http://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html). PuTTY emulates a terminal, making it possible to use a Linux terminal on Windows. 
Choose 32-bit or 64-bit **_putty.exe_** under **Alternative binary files**.


Log in as `noob@tutorial.tghack.no` for the tutorial, as shown on the picture below. The password is **_noob123_**.

<a href="https://imgur.com/kFyEGMr"><img src="https://i.imgur.com/kFyEGMr.png?1" title="source: imgur.com" /></a>

For Linux and mac OS, you can use the **ssh** command: `ssh noob@tutorial.tghack.no`, with password **_noob123_**.


### First steps
One of the most important skills is being able to navigate the file structure. On Linux, folders are called directories. Here's a list of the most important commands:

* `ls` - list files in the current directory

* `cd` - change directory

* `cat` - read files

* `pwd` - print current directory (present working directory)


Let's test them on the server:
```
tutorial@kosekroken:~$ ls
some_directory this_is_a_file
tutorial@kosekroken:~$ pwd
/home/tutorial
tutorial@kosekroken:~$ cd some_directory/
tutorial@kosekroken:~/some_directory$ ls
tutorial@kosekroken:~/some_directory$ pwd
/home/tutorial/some_directory
tutorial@kosekroken:~/some_directory$ cd ..
tutorial@kosekroken:~$ pwd
/home/tutorial
tutorial@kosekroken:~$ cat this_is_a_file
hack the planet
tutorial@kosekroken:~$
```


All of the above commands take different **arguments**. Arguments are text following a command specifying certain aspects of the command. If there are several arguments, they are separated by spaces. To find out how a command works, we can use another command called **man**. When using the command **man**, you are looking through the manual pages, also referred to as _man pages_. You may look through the man page by passing the name of a command as an argument to the **man** command, for instance you may enter `man ls` into your terminal. Another way is to pass the **_-h_** or **_--help_** flag as an argument to the program, which usually prints usage information.


The command **ls** can be used without any arguments, and it list all the files in the current directory. We always start in our home folder, located at **_/home/<username\>_**, when logging in.

If we want to change to another directory, we can simply write **cd <name\>**, where **cd** is the command, and **<name\>** is its argument, the name of the folder you want to change to. However, there's a special command we can use to change to the folder **_above_** the one we're currently in: **_cd .._**. Two special directories, `.` and `..`, means the current directory, and the parent directory, respectively.

The command **cat** can be used to view the contents of a file. To use the command, add the file you want to see the contents of as an argument. For example `cat this_is_a_file`.


If you want to search for a certain word in a file (or in the output of another command), we can use the command **grep**. In the illustration below, the first argument after the command is the word we are looking for, **secret** in this case. The following argument, **secret.txt**, is the file we are looking in. 

```
tutorial@kosekroken:~$ grep secret secret.txt
supersecret: #secrets
```


We can also use it together with command output. For this to work, we can glue two or more commands together with a pipe `|`. When using pipe, we have several commands divided by the pipe sign `|`. Pipe takes the output from the command to the left of the pipe and passes it to the command to the right of the pipe. In the below example, we use **cat** to output the contents of the file **secret.txt**, and then we use **grep** to find the word **secret** in the output from **secret.txt**. 
```
tutorial@kosekroken:~$ cat secret.txt | grep secret
supersecret: #secrets
```


This will be useful later, for example when looking through log files. And remember that you now know how to use `man` (or google) to obtain information about commands that are unknown for you. Google and searching is still the most powerful tool of becoming a hacker!


### Hidden files
Linux supports hidden files. They are created by adding a `.` in front of the name. To create a hidden file called **secret.txt**, we could do this: `touch .secret.txt`. We can't see it by using **ls** without any arguments, but **ls** has an option to view all files starting with `.`!
```
noob@tghack: touch .secret.txt
noob@tghack: ls
[no output]
noob@tghack: ls -a
. .. .secret.txt
```


There's a hidden file on the tutorial server as well:
```
tutorial@kosekroken:~$ ls -a
. .. .bash_logout .bashrc .hidden_file .profile secret.txt some_directory this_is_a_file .viminfo
```
**.hidden_file** is a file we've created. There are many other hidden files in our home directory however, usually referred to as **_dotfiles_**. **.bash_logout**, **.bashrc**, **.profile**, and **.viminfo** are all files you'd usually see on a Linux system. If you are curious about why they are there, try using google.


If we want to get an overview of the file structure, we can use the `tree` command. It's also possible to make **ls** list all files in subdirectories: `ls *`.

```
tutorial@kosekroken:~$ tree
.
├── secret.txt
├── some_directory
└── this_is_a_file

1 directory, 2 files
```


### Users
To check who you are logged in as, run `whoami`. Information about other users on the system is located in **/etc/passwd**. Similarly, group information is stored in **/etc/groups**. You can also use `id` to see user id and group ids.

```
tutorial@kosekroken:~$ whoami
tutorial
tutorial@kosekroken:~$ id
uid=1002(tutorial) gid=1003(tutorial) groups=1003(tutorial)
```


### System logs etc.
To view kernel log information, use `dmesg`.


### Archive formats
Linux has tools to work with all the common archive formats, like zip, rar, and gz.

* unrar

* unzip

* tar

* gunzip


Use the **man pages** (`man unrar`, `man unzip`, etc.) to figure out how the programs work. Another tip is to use the command **file** to find out what kind of file it is. For instance, there may be a zip-file without the suffix **.zip**. The command **file** will tell you that it is an zip-file.


### Sorting
You need three commands for this level: `sort`, `uniq`, and `grep`. Use google and the manual pages to solve the tasks! Here's some basic usage of the commands:

```
tutorial@kosekroken:~/sorting$ cat sortme.txt
Cool
Awesome
Fantastic
Super
Nice
tutorial@kosekroken:~/sorting$ sort sortme.txt
Awesome
Cool
Fantastic
Nice
Super
tutorial@kosekroken:~/sorting$ cat duplicates.txt
Pineapple
Pineapple
Melon
Banana
Banana
Apple
Mango
Mango
tutorial@kosekroken:~/sorting$ uniq duplicates.txt
Pineapple
Melon
Banana
Apple
Mango
```
