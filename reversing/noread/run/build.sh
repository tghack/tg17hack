#!/bin/bash

# lol
cd ../src && make
cd ../run
docker kill noread
docker rm noread
docker build -t tg17/noread .
