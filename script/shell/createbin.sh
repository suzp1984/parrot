#!/bin/sh

PATH=.:$PATH
RPM=XXXX.rpm
LICENSEBIN=YYYY.bin
BASE=$(basename $RPM .rpm)
sum=`sum $RPM`
index=1
for s in $sum
do
	case $index in
		1) sum1=$s;
			index=2;
			;;
		2) sum2=$s;
			index=3;
			;;
	esac
done
cat $LICENSEBIN | sed -e s/OUTNAME/$RPM/ -e s/SUM1/$sum1/ -e s/SUM2/$sum2/ > linux_license_new.bin
dos2unix -k -q linux_license_new.bin
cat linux_license_new.bin $RPM > ${BASE}-rpm.bin
sudo chmod a+x res/${BASE}-rpm.bin
