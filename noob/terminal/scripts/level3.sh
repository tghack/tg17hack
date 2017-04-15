#!/bin/bash

# 1.
sudo useradd user_flag_y000 -s /bin/false

# 2.
sudo groupadd this_is_the_group_fl4g

# 3.
cd log
make
sudo insmod log.ko
cd ..
