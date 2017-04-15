#!/bin/bash
./mkcert.sh
./proxy 127.0.0.1:5367 tghack.no:1287 &
hosts=`cat /etc/hosts`
echo 127.0.0.1 tghack.no >> /etc/hosts
./run_client.sh
echo "$hosts" > /etc/hosts
