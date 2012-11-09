#!/bin/bash

APPEND="halfsize"

for file in `ls *.JPG *.jpg`
do
	convert -resize 50%x50% ${file} ${APPEND}_${file}
done
