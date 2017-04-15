#!/bin/bash

echo "TG17{s0_hidden}" > .flag.txt
mkdir -p .hidden/more_hidden/super_hidden
echo "TG17{g0_away_please}" > .hidden/more_hidden/super_hidden/.flag.txt

# tree
RAND_DIR="/home/noob/."$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 12 | head -n 1)
mkdir $RAND_DIR

for i in `seq 1 10`;
do
	RAND=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 12 | head -n 1)
	mkdir $RAND_DIR/$RAND
done

echo "TG17{birds_eye_view}" > $RAND_DIR/$RAND/cat_me.txt
