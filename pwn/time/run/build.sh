#!/bin/bash

docker kill time
docker rm time
cp ../src/time.c .
cp ../flag.txt .
docker build -t tg17/time .
rm time.c
rm flag.txt
