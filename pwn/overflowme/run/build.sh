#!/bin/bash

docker kill overflow
docker rm overflow
docker build -t tg17/overflow .
