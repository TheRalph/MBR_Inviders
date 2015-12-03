#!/bin/bash

if [ $# != 2 ]
then
    echo "Syntax: $0 [file_bs.bin] [device]"
    echo "example to write 'test_bs.bin' to the Bootstrap of 'sda':"
    echo "$0 test sda"

    exit 1
fi

if [ "$(id -u)" != "0" ]
then
   echo "This script must run as root" 1>&2
   exit 1
fi

disk=$2
bsFile=$1_bs.bin

if [ -f $bsFile ]
then
    # check file size
    fileSize=`stat -c %s $bsFile`

    if [ $fileSize == 446 ]
    then
        echo "loading bootstrap binary file from $bsFile to $disk"
        dd if=$bsFile of=/dev/$disk bs=$fileSize count=1
    else
        echo "Size of file '$bsFile' is $fileSize, but 446 o is expected"
        exit 1
    fi
else
    echo "File '$bsFile' not found"
    exit 1
fi

exit 0
