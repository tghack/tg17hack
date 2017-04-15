package main

import (
	"bytes"
	"crypto/hmac"
	"crypto/sha256"
	"crypto/tls"
	"encoding/hex"
	"fmt"
)

const seed = "43P8dmMaN0RE3ybl9ImA7y"

func main() {
	conn, err := tls.Dial("tcp", "tls.tghack.no:5367", nil)
	if err != nil {
		fmt.Println(err)
	} else {
		var buf []byte = make([]byte, 256, 256)
		size, _ := conn.Read(buf)

		var tmpBytes []byte = make([]byte, size, size)

		for i := 0; i < size; i++ {
			tmpBytes[i] = buf[i]
		}

		split := bytes.Split(tmpBytes, []byte("="))
		var challenge []byte

		if (len(split) == 2) && (string(split[0]) == "challenge") {
			challenge = split[1]
		} else {
			conn.Close()
			return
		}
		mac := hmac.New(sha256.New, []byte(seed))
		mac.Write(challenge)

		hashSum := hex.EncodeToString(mac.Sum(nil))

		conn.Write([]byte("response=" + hashSum))

		conn.Close()
	}
}
