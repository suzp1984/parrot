#!/bin/sh
PATH=/usr/bin:/bin
umask 022
echo_args="-e "
localinstall=$1
more <<"EOF"
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx
EOF
agreed=
while [ x$agreed = x ]; do
	echo
	echo "Do you agree to the above license terms? [yes or no] "
	read reply leftover
	case $reply in
		y* | Y*)
			agreed=1;;
		n* | N*)
			echo "If you don't agree to the license you can't install this software";
			exit 1;;
	esac
done
if [ -d "$localinstall" ] ; then
	outname=$localinstall/OUTNAME
else
	outname=OUTNAME
fi
echo "Unpacking..."
tail -n +AAA $0 > $outname    
if [ -x /usr/bin/sum ] ; then
	echo "Checksumming..."

	sum=`/usr/bin/sum $outname`
	index=1
	for s in $sum
	do
		case $index in
			1)  sum1=$s;
				index=2;
				;;
			2)  sum2=$s;
				index=3;
				;;
		esac
	done
	if [ $sum1 != SUM1 -o $sum2 != SUM2 ] ; then
		echo "The download file appears to be corrupted."
		echo "Please do not attempt to install this archive file."
		exit 1
	fi
else
	echo "Can't find /usr/bin/sum to do checksum.  Continuing anyway."
fi
echo "Done."
exit 0

