#include "platform.h"

    .equ STACK_SIZE, 1024

    .global _boot

    .text
_boot:
    csrr t0, mhartid
    mv tp, t0
    bnez t0, park // Just one hart

    la a0, _bss_start
    la a1, _bss_end
    bgeu a0, a1, 2f

1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
    
2:
    slli t0, t0, 10
    la sp, stacks + STACK_SIZE
    add sp, sp, t0


    # https://lore.kernel.org/qemu-devel/20201223192553.332508-1-atish.patra@wdc.com/
	# For qemu version >= 6.0, exception would be raised if no PMP enty is
	# configured. So just configure one entny, which allows all the whole
	# 32-bits physical address range is R/W/X.
	# FIXME: I say it is a temporary workaroud due to I think the patch
	# above contains bug, and I have raised new issue to qemu but it has not
	# been rootcaused till now. Details please refer to
	# https://gitlab.com/qemu-project/qemu/-/issues/585 or
	# https://gitee.com/unicornx/riscv-operating-system-mooc/issues/I441IC (in chinese)
	# So it's just a temporary workaround till now to not block people who
	# want to try newer qemu (>= 6.0).
	li      t0, 0xffffffff
	csrw    pmpaddr0, t0
	li      t0, 0xf
	csrw    pmpcfg0, t0
    # At the end of start_kernel, schedule() will call MRET to switch
	# to the first task, so we parepare the mstatus here.
	# Notice: default mstatus is 0
	# Set mstatus.MPP to 3, so we still run in Machine mode after MRET.
	# Set mstatus.MPIE to 1, so MRET will enable the interrupt.
	# li	t0, 3 << 11 | 1 << 7
	li	t0, 1 << 7 # Just support u mode
	csrr	a1, mstatus
	or	t0, t0, a1
	csrw	mstatus, t0

    j start_kernel

park:
    wfi
    j park

stacks:
    .skip STACK_SIZE * MAXNUM_CPU
    .end
