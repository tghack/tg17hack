#!/bin/bash
openssl req -x509 -nodes -newkey rsa:2048 -keyout key.pem -out cert.pem
