package main

import (
	"bytes"
	"crypto/hmac"
	"crypto/rand"
	"crypto/sha256"
	"crypto/tls"
	"encoding/hex"
	"fmt"
	"net"
	"os"
	"strings"
)

const seed = "43P8dmMaN0RE3ybl9ImA7y"

var cert tls.Certificate
var certErr error

func getCert(_ *tls.ClientHelloInfo) (*tls.Certificate, error) {
	return &cert, certErr
}

func handle(conn net.Conn, err error) {
	if err != nil {
		fmt.Println(err)
		return
	}
	var tmp []byte = make([]byte, 32, 32)
	_, err = rand.Read(tmp)

	var challenge []byte = []byte(hex.EncodeToString(tmp))

	mac := hmac.New(sha256.New, []byte(seed))
	mac.Write(challenge)

	var expectedResp []byte = []byte(hex.EncodeToString(mac.Sum(nil)))

	conn.Write([]byte("challenge=" + string(challenge)))

	var buf []byte = make([]byte, 256, 256)

	size, _ := conn.Read(buf)
	var tmpBytes []byte = make([]byte, size, size)
	for i := 0; i < size; i++ {
		tmpBytes[i] = buf[i]
	}

	split := bytes.Split(tmpBytes, []byte("="))

	var response []byte

	if (len(split) == 2) && (string(split[0]) == "response") {
		response = split[1]
	} else {
		conn.Close()
		fmt.Println("conn closed")
		return
	}
	if hmac.Equal(expectedResp, response) {
		conn.Write([]byte("Token: TG17{thats_how_you_break_ssl}"))
	}

	conn.Close()
}

func main() {
	if len(os.Args) != 4 {
		fmt.Printf("syntax: %s cert key [ port | address ]\n", os.Args[0])
		return
	}

	cert, certErr = tls.LoadX509KeyPair(os.Args[1], os.Args[2])
	cfg := &tls.Config{GetCertificate: getCert}
	var listener string

	if strings.ContainsRune(os.Args[3], ':') {
		listener = os.Args[3]
	} else {
		listener = fmt.Sprintf(":%s", os.Args[3])
	}

	sock, err := tls.Listen("tcp", listener, cfg)
	if err != nil {
		fmt.Println(err)
		return
	}
	for {
		conn, err := sock.Accept()
		go handle(conn, err)
	}
	sock.Close()
}
