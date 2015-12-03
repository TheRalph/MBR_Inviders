#!/bin/bash

if [ $# != 2 ]
then
    echo "Syntax: $0 [file_mbr.bin] [device]"
    echo "example to write 'test_mbr.bin' to the MBR of 'sda':"
    echo "$0 test sda"

    exit 1
fi

if [ "$(id -u)" != "0" ]
then
   echo "This script must run as root" 1>&2
   exit 1
fi

disk=$2
mbrFile=$1_mbr.bin

if [ -f $mbrFile ]
then
    # check file size
    fileSize=`stat -c %s $mbrFile`

    if [ $fileSize == 512 ]
    then
        echo "loading mbr binary file from $mbrFile to $disk"
        dd if=$mbrFile of=/dev/$disk bs=$fileSize count=1
    else
        echo "Size of file '$mbrFile' is $fileSize, but 512 o is expected"
        exit 1
    fi
else
    echo "File '$mbrFile' not found"
    exit 1
fi

exit 0
