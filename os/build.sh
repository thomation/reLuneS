riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv64ima -mabi=lp64 -g -Wall boot.S kernel.c -Ttext=0x80000000 -o os.elf
