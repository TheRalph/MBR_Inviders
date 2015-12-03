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
bsFile=${disk}_bs.bin

if [ $# == 2 ]
then
    bsFile=$2_bs.bin
fi

echo "saving mbr bootstrap from $disk to $bsFile"

dd if=/dev/$disk of=$bsFile bs=446 count=1

exit 0
