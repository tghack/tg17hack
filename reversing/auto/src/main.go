package main

import (
	"encoding/base64"
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"net"
	"os"
	"os/exec"
	"time"
)

const str_template = "#include <stdio.h>\n" +
	"#include <string.h>\n" +
	"#include <stdlib.h>\n\n" +
	"int main(void)\n{\n" +
	"\tchar buf[64] = { 0 };\n\n" +
	"\tprintf(\"Password: \");\n" +
	"\tif (!fgets(buf, sizeof(buf), stdin))\n" +
	"\t\texit(EXIT_FAILURE);\n" +
	"\tbuf[strcspn(buf, \"\\n\")] = '\\0';\n" +
	"\tif (!strcmp(buf, \"%s\"))\n" +
	"\t\tputs(\"You did it!!!\\n\");\n" +
	"\telse\n" +
	"\t\tputs(\"Try again :(\\n\");\n" +
	"\treturn 0;\n}\n"

const letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

const flag = "TG17{bin4ries_3verywhere_wh4t_t0_d0}"

func makeClients(listener net.Listener) chan net.Conn {
	ch := make(chan net.Conn)
	i := 0

	go func() {
		for {
			client, err := listener.Accept()
			if err != nil {
				fmt.Printf("Couldn't accept client: %v\n", err)
				continue
			}
			i++
			fmt.Printf("%d: %v -> %v\n", i, client.LocalAddr(), client.RemoteAddr())
			ch <- client
		}
	}()

	return ch
}

func randomString(n int) string {
	b := make([]byte, n)
	for i := range b {
		b[i] = letters[rand.Int63()%int64(len(letters))]
	}
	return string(b)
}

func randomChar() byte {
	return letters[rand.Int63()%int64(len(letters))]
}

func randomLen() int {
	return int(rand.Int63()%int64(5)) + 5
}

func handleClient(c net.Conn) {
	defer c.Close()
	for i := 0; i < 10; i++ {
		password := sendBinary(c)
		c.Write([]byte("Password: "))

		timeout := make(chan int, 1)
		buf := make([]byte, len(password))

		t0 := time.Now()

		go func() {
			n, err := c.Read(buf)
			if err != nil {
				return
			}

			timeout <- n
		}()

		select {
		case n := <-timeout:
			t1 := time.Now()
			fmt.Printf("Solved time: %v\n", t1.Sub(t0))
			fmt.Printf("[+] Received %d bytes: %s\n", n, string(buf))
			if string(buf) == password {
				fmt.Printf("[+] password correct\n")
				c.Write([]byte("Correct! Have another one!\n"))
			} else {
				fmt.Printf("[-] incorrect!\n")
				c.Write([]byte("Incorrect! :(\n"))
				return
			}
		case <- time.After(time.Second * 7):
			fmt.Printf("[-] User too slow!\n")
			c.Write([]byte("\nToo slow!\n"))
			return
		}
	}

	// User solved all binaries
	c.Write([]byte("Congrats! The flag is " + flag))
}

func sendBinary(c net.Conn) string {
	tmpfile, err := ioutil.TempFile("./", "code_")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("[+] Filename: %s\n", tmpfile.Name())

	name := tmpfile.Name()
	binfile := name + ".bin"
	defer os.RemoveAll(name)
	defer os.RemoveAll(binfile)

	tmp1 := randomString(randomLen())
	fmt.Printf("[+] random string:    %s\n", tmp1)
	_, err = fmt.Fprintf(tmpfile, str_template, tmp1)
	if err != nil {
		fmt.Printf("shit: %v\n", err)
	}

	cmd := exec.Command("gcc", "-fpic", "-x", "c", name, "-o", binfile)
	err = cmd.Run()
	if err != nil {
		log.Fatal(err)
	}

	file_buf, err := ioutil.ReadFile(binfile)
	if err != nil {
		log.Fatal(err)
	}

	buf := make([]byte, base64.StdEncoding.EncodedLen(len(file_buf)))
	base64.StdEncoding.Encode(buf, file_buf)

	c.Write(buf)
	c.Write([]byte("\nFresh base64 for you <3\n"))
	return tmp1
}

func main() {
	rand.Seed(time.Now().UnixNano())

	server, err := net.Listen("tcp", ":2270")
	if err != nil {
		panic("Listen fail: " + err.Error())
	}
	clients := makeClients(server)
	for {
		go handleClient(<-clients)
	}
}
