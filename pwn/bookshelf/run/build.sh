#!/bin/bash

docker kill book
docker rm book
docker build -t tg17/book .
