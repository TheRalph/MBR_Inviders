MBR_NAME=myBoot_bs.bin

all: $(MBR_NAME)

$(MBR_NAME): myBoot.asm ../mydrawM.inc ../mydrawF.inc ../mainInva.inc
	nasm -o $@ -f bin $<

run: $(MBR_NAME)
	qemu ./$(MBR_NAME)

clean:
	rm -f $(MBR_NAME) *~ *.*~

