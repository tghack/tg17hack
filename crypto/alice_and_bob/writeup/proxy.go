package main

import("fmt"
       "crypto/tls"
       "net"
       "os"
       "sync")

var mutex = &sync.Mutex{}
var wg sync.WaitGroup

var cert tls.Certificate
var certErr error

func getCert(_ *tls.ClientHelloInfo)(*tls.Certificate, error){
    return &cert, certErr
}

//From client to server
func handleRead(clientConn net.Conn, serverConn net.Conn){
    for i:=0; i<10; i++{
        var buf []byte = make([]byte, 256, 256)
        size,_ := clientConn.Read(buf)
        printSync("client: %s\n", string(buf))
        serverConn.Write(buf[:size])
    }
    wg.Done()
}
//From server to client
func handleWrite(clientConn net.Conn, serverConn net.Conn){
    for i:=0; i<10; i++{
        var buf []byte = make([]byte, 256, 256)
        size,_ := serverConn.Read(buf)
        printSync("server: %s\n", string(buf))
        clientConn.Write(buf[:size])
    }
    wg.Done()
}

func printSync(format string, txt string){
    if (txt[0] <= 32) || (txt[0] >= 127){
        return
    }
    mutex.Lock()
    fmt.Printf(format, txt)
    mutex.Unlock()
}

func main(){
    if len(os.Args) != 3{
        fmt.Printf("syntax: %s client server", os.Args[0])
        return
    }

    cert, certErr = tls.LoadX509KeyPair("cert.pem", "key.pem")
    cfg := &tls.Config{GetCertificate: getCert}

    sock, err := tls.Listen("tcp", os.Args[1], cfg)
    if err != nil{
        fmt.Println(err)
        return
    }
    clientConn,_ := sock.Accept()

    serverConn, err := tls.Dial("tcp", os.Args[2], &tls.Config{InsecureSkipVerify: true})
    if err != nil{
        fmt.Println(err)
        return
    }
    wg.Add(2)
    go handleWrite(clientConn, serverConn)
    go handleRead(clientConn, serverConn)
    wg.Wait()
}
