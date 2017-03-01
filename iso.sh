#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/ginkgos.kernel isodir/boot/ginkgos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "ginkgos" {
	multiboot /boot/ginkgos.kernel
}
EOF
grub-mkrescue -o ginkgos.iso isodir
