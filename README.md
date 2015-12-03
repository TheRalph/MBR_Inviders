# MBR_Inviders

#*******************************************************************************
# Structure of a classical generic MBR
#*******************************************************************************

| Address | Offset | Description          | Size |
|:---------|:--------|:----------------------|------:|
| +000h   | +000   | Bootstrap code area  | 446  |
| +1BEh   | +446   | Partition entry #1	  |  16  |
| +1CEh   | +462   | Partition entry #2	  |  16  |
| +1DEh   | +478   | Partition entry #3	  |  16  |
| +1EEh   | +494   | Partition entry #4	  |  16  |
| +1FEh   | +510   | 55h Boot signature	  |   1  |
| +1FFh   | +511   | AAh                  |   1  |

Total size: 446 + 4×16 + 2 = 512

#***************************************************************
# Save MBR
#***************************************************************

Before writing your code to the MBR of a drive, you need to save the existing one.

- To save the MBR of a drive
sudo dd if=/dev/[driveToSave] of=[binarySaveOfMBR] bs=512 count=1

- Example to save the MBR of drive "sda" into the file "sda_mbr.bin"
sudo dd if=/dev/sda of=sda_mbr.bin bs=512 count=1

you can also use the script "extractMBR.sh" to save the MBR of drive "sda" into the file "test_mbr.bin"
Ex: sudo ./extractMBR.sh sda test

- To save only the Bootstrap code area, you can use the following line:
sudo dd if=/dev/[driveToSave] of=[binarySaveOfMBR] bs=446 count=1

- Example to save the Bootstrap code area of drive "sda" into the file "test_bs.bin"
sudo dd if=/dev/sda of=test_bs.bin bs=446 count=1

you can also use the script "extractBootstrap.sh" to save the Bootstrap code area of drive "sda" into the file "test_bs.bin"
Ex: sudo ./extractMBR.sh sda test

#***************************************************************
# Writing code to the MBR
#***************************************************************

WARNING: use all that commands at you own risk !

WARNING: writing a full binary file of 512 bytes on the MBR of a disk will overwrite the partition table of that disk.
If you do not want to overwrite the partition table, do not write more than 446 bytes on the MBR, that is only the bootstrap part.

- To write a binary MBR file to the MBR of a drive
sudo dd if=[binaryMBRFile] of=/dev/[driveToWriteTo]  bs=512 count=1

- Example to write the file "sda_mbr.bin" to the MBR of drive "sda"
sudo dd if=sda_mbr.bin of=/dev/sda bs=512 count=1

you can also use the script "writeMBR.sh" to write the file "test_mbr.bin" to the MBR of drive "sda"
Ex: sudo ./writeMBR.sh test sda

- To write only on the Bootstrap code area, you can use the following line:
sudo dd if=[binaryBSFile] of=/dev/[driveToWriteTo] bs=446 count=1

- Example to write the file "test_bs.bin" to the Bootstrap code area of drive "sda"
sudo dd if=test_bs.bin of=/dev/sda bs=446 count=1

you can also use the script "extractBootstrap.sh" to write the file "test_bs.bin" to the Bootstrap code area of drive "sda"
Ex: sudo ./writeMBR.sh test sda

#***************************************************************
# Disassemble the MBR bootstrap code area
#***************************************************************
objdump -D -b binary -M intel -mi386 -Maddr16,data16 sda_mbr.bin

