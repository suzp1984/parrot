#!/bin/bash
for file in `adb shell ls /system/app/*.apk`; do
	#echo $file
	realname=`echo $file | dos2unix`
	filedirname=`echo $realname | sed -e 's/\/system\/app\///' -e 's/.apk//'`
	adb pull $realname &&  unzip $filedirname.apk -d $filedirname
done
