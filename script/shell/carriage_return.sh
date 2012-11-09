#!/bin/bash
#Carriage return '\r' test

num=1
while [ ${num} -le 50 ]
do
	echo -n -e "Node ${num}\r" > /dev/tty
	sleep 1
	num=$(($num + 1))
done
