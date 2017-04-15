#!/bin/bash

echo "TG17{c0mpressi0n_101}" > /tmp/tmp_flag.txt
tar cvf /home/noob/level4/flag.tar.gz /tmp/tmp_flag.txt

echo "TG17{unzip_me_please}" > /tmp/tmp_flag.txt
# TODO: fix zip
zip /tmp/tmp_flag.txt -o /home/noob/level4/flag.zip

echo "TG17{we_d0nt_need_no_winrar}" > /tmp/tmp_flag.txt
rar /tmp/tmp_flag.txt -o /home/noob/level4/flag.rar

rm /tmp/tmp_flag.txt
