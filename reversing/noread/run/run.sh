#!/bin/bash

docker run --security-opt seccomp:unconfined -it --rm --name noread --user 0 -p 2222:22 tg17/noread
