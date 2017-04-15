#!/bin/bash

docker kill leakme
docker rm leakme
docker build -t tg17/leakme .
