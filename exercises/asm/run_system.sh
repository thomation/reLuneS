riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv64ima -mabi=lp64 -g -Wall  add.s -Ttext=0x80000000 -o add.elf
riscv64-unknown-elf-objcopy -O binary add.elf add.bin
qemu-system-riscv64 -nographic -smp 1 -machine virt -bios none -kernel add.elf -s -S &
gdb-multiarch add.elf -q -x gdbinit 
