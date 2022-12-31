SRCS_ASM = \
	boot.S \

SRCS_C = \
	kernel.c \
	uart.c \

OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

CC = riscv64-unknown-elf-gcc
#CFLAGS = -nostdlib -fno-builtin -march=rv64ima -mabi=lp64 -g -Wall
CFLAGS = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} ${CFLAGS} -Ttext=0x80000000 -o os.elf $^

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<