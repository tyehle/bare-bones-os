.PHONY : clean

iso : binary
	grub-file --is-x86-multiboot myos.bin || (echo "the file is not multiboot"; exit 1)
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	rm -r isodir

binary : boot.o kernel.o linker.ld
	i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

boot.o : boot.s
	i686-elf-as boot.s -o boot.o

kernel.o : kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

clean :
	if [ -d isodir ]; then rm -r isodir; fi
	rm *.iso *.bin *.o
