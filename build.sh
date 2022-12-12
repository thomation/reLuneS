nasm ./os/boot.asm -o ./build/boot.bin
dd if=./build/boot.bin of=./build/a.img bs=512 count=1