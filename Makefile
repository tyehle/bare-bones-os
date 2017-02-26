BIN = ginkgos.bin
ISO = ginkgos.iso

OBJ = boot.o kernel.o

.PHONY : clean

run-iso : iso
	qemu-system-i386 -cdrom $(ISO)

run-bin : $(BIN)
	qemu-system-i386 -kernel $(BIN)

iso : $(BIN)
	grub-file --is-x86-multiboot $(BIN) || (echo "the file is not multiboot"; exit 1)
	mkdir -p isodir/boot/grub
	cp $(BIN) isodir/boot/$(BIN)
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir
	rm -r isodir

$(BIN) : $(OBJ) linker.ld
	i686-elf-gcc -T linker.ld -o $(BIN) -ffreestanding -O2 -nostdlib $(OBJ) -lgcc

%.o : %.s
	i686-elf-as $< -o $@

%.o : %.c
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

clean :
	if [ -d isodir ]; then rm -r isodir; fi
	rm *.iso *.bin *.o
