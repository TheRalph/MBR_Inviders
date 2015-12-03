#!/bin/bash

if [ $# != 1 -a $# != 2 ]
then
    echo "Syntax:  $0 [device] [fileName]"
    echo "example: $0 sda test"

    exit 1
fi

if [ "$(id -u)" != "0" ]
then
   echo "This script must run as root" 1>&2
   exit 1
fi

disk=$1
mbrFile=${disk}_mbr.bin

if [ $# == 2 ]
then
    mbrFile=$2_mbr.bin
fi

echo "saving mbr bootstrap from $disk to $mbrFile"

dd if=/dev/$disk of=$mbrFile bs=512 count=1

exit 0
