#!/bin/bash

NUM=1
name1="a"
name2="b"

val="test"

eval "prev_${name1}_${name2}=$val"
echo "---------------------"
val="name"
eval "echo \${prev_${name1}_${name2}}"

