#!/bin/bash

# TODO: generate random strings
# then place flag
# TG17{Let's_s0rt_this_out}

# sort out.txt gives flag
python2 gen_random.py

# uniq -u uniq.txt gives flag
python2 gen_uniq.py

# grep "TG17" grep.txt gives flag
python2 gen_grep.py
