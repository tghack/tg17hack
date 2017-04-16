package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strconv"
	"strings"
)

var htmlFile []byte
var jsFile []byte

func main() {
	var err error

	htmlFile, err = ioutil.ReadFile("deny.html")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	jsFile, err = ioutil.ReadFile("geo.js")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	var port int64 = 7691

	if len(os.Args) >= 2 {
		port, err = strconv.ParseInt(os.Args[1], 10, 64)
		if (err != nil) || (port > 65535) || (port <= 0) {
			fmt.Printf("%s is not a valid port number!\n", os.Args[1])
			os.Exit(1)
		}
	}
	http.HandleFunc("/", locationHandler)
	http.HandleFunc("/geo.js", jsHandler)
	fmt.Printf("Listening on port %d.\n", port)
	http.ListenAndServe(fmt.Sprintf(":%d", port), nil)
}

func locationHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/html; charset=utf8")
	if validate(r.RequestURI) {
		w.Write([]byte("Token: TG17{fun_with_maps}"))
		return
	}
	w.Write(htmlFile)
}
func jsHandler(w http.ResponseWriter, _ *http.Request) {
	w.Header().Set("Content-Type", "application/javascript; charset=utf8")
	w.Write(jsFile)
}

func validate(pos string) bool {
	var lon int64
	var lat int64
	f := func(c rune) bool {
		return (c == '&') || (c == '?') || (c == '.') || (c == ',')
	}
	var parts []string = strings.FieldsFunc(strings.ToLower(pos), f)

	var hasLon bool
	var hasLat bool
	//Parsing query
	for _, v := range parts {
		var splitted []string = strings.Split(v, "=")
		if len(splitted) == 2 {
			var err error
			if (splitted[0] == "lon") && !hasLon {
				lon, err = strconv.ParseInt(splitted[1], 10, 64)
				if err == nil {
					hasLon = true
				}
			} else if (splitted[0] == "lat") && !hasLat {
				lat, err = strconv.ParseInt(splitted[1], 10, 64)
				if err == nil {
					hasLat = true
				}
			}
			if hasLon && hasLat {
				break
			}
		}
	}

	if hasLon && hasLat {
		var validLat bool = (lat > -1) && (lat < 1)
		var validLon bool = (lon > 165) && (lon < 168)
		return (validLon && validLat)
	}
	return false
}
