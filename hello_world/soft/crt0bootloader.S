/*
 * LatticeMico32 C startup code.
 * Contributed by Jon Beniston <jon@beniston.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* From include/sys/signal.h */
#define	SIGINT	2	/* interrupt */
#define	SIGTRAP	5	/* trace trap */
#define	SIGFPE	8	/* arithmetic exception */
#define	SIGSEGV	11	/* segmentation violation */

/* Exception handlers - Must be 32 bytes long. */

        .section    .boot, "ax", @progbits
        .weak       IRQ_handler

        .global     _start
_start:
_reset_handler:
        /* Clear r0 */
        xor     r0, r0, r0
        wcsr    IE, r0
        wcsr    IM, r0
        mvhi    r1, hi(_reset_handler)
        ori     r1, r1, lo(_reset_handler)
        wcsr    EBA, r1
        bi      _crt0
        nop

_breakpoint_handler:
        sw      (sp+0), ra
        calli   _save_all
        mvi     r1, SIGTRAP
        calli   raise
        bi      _restore_all_and_return
        nop
        nop
        nop

_instruction_bus_error_handler:
        sw      (sp+0), ra
        calli   _save_all
        mvi     r1, SIGSEGV
        calli   raise
        bi      _restore_all_and_return
        nop
        nop
        nop

_watchpoint_handler:
        sw      (sp+0), ra
        calli   _save_all
        mvi     r1, SIGTRAP
        calli   raise
        bi      _restore_all_and_return
        nop
        nop
        nop

_data_bus_error_handler:
        sw      (sp+0), ra
        calli   _save_all
        mvi     r1, SIGSEGV
        calli   raise
        bi      _restore_all_and_return
        nop
        nop
        nop

_divide_by_zero_handler:
        sw      (sp+0), ra
        calli   _save_all
        mvi     r1, SIGFPE
        calli   raise
        bi      _restore_all_and_return
        nop
        nop
        nop

_interrupt_handler:
        bi		vPortInterruptHandler
        nop
        nop
        nop
        nop
        nop
        nop
        nop

_system_call_handler:
        sw      (sp+0), ra
        calli   _save_all
        mv      r1, sp
        calli   _handle_scall
        bi      _restore_all_and_return
        nop
        nop
        nop

        /* Program starts here */

_crt0:

        /* Setup stack and global pointer */
        mvhi    sp, hi(_fstack)
        ori     sp, sp, lo(_fstack)
        mvhi    gp, hi(_gp)
        ori     gp, gp, lo(_gp)

        /* Relocate text */
        mvhi    r1, hi(_ftext)
        ori     r1, r1, lo(_ftext)
        mvhi    r2, hi(_ftext_rom)
        ori     r2, r2, lo(_ftext_rom)
        be      r1, r2, .L_done_text_relocation
        mvhi    r3, (_etext)
        ori     r3, r3, (_etext)
        calli   _relocate_text
.L_done_text_relocation:

        /* Relocate read-only data */
        mvhi    r1, hi(_frodata)
        ori     r1, r1, lo(_frodata)
        mvhi    r2, hi(_frodata_rom)
        ori     r2, r2, lo(_frodata_rom)
        be      r1, r2, .L_done_rodata_relocation
        mvhi    r3, hi(_erodata)
        ori     r3, r3, lo(_erodata)
        sub     r3, r3, r1
        calli   memcpy
.L_done_rodata_relocation:

        /* Relocate data */
        mvhi    r1, hi(_fdata)
        ori     r1, r1, lo(_fdata)
        mvhi    r2, hi(_fdata_rom)
        ori     r2, r2, lo(_fdata_rom)
        be      r1, r2, .L_done_data_relocation
        mvhi    r3, hi(_edata)
        ori     r3, r3, lo(_edata)
        sub     r3, r3, r1
        calli   memcpy
.L_done_data_relocation:

        /* Clear BSS */
        mvhi    r1, hi(_fbss)
        ori     r1, r1, lo(_fbss)
        mvi     r2, 0
        mvhi    r3, hi(_ebss)
        ori     r3, r3, lo(_ebss)
        sub     r3, r3, r1
        calli   memset

        /* Intialise any hardware */
        calli   _init_hw

        /* Call C++ constructors */
        calli   _init

        /* Call C++ destructors on exit */
        mvhi    r1, hi(_fini)
        ori     r1, r1, lo(_fini)
        calli   atexit

        /* Call main program */
        mvi     r1, 0
        mvi     r2, 0
        mvi     r3, 0
        calli   main

        /* Call exit, which doesn't return, to perform any clean up */
        calli   exit

        /* Relocate text section. We can't use memcpy, as that might be
           in the memory we're relocating. */
_relocate_text:
        lb      r4, (r2+0)
        addi    r3, r3, 1
        sb      (r1+0), r4
        addi    r1, r1, 1
        bne     r1, r3, _relocate_text
        ret

        /* Save all registers onto the stack */
_save_all:
        addi    sp, sp, -132
        sw      (sp+4), r1
        sw      (sp+8), r2
        sw      (sp+12), r3
        sw      (sp+16), r4
        sw      (sp+20), r5
        sw      (sp+24), r6
        sw      (sp+28), r7
        sw      (sp+32), r8
        sw      (sp+36), r9
        sw      (sp+40), r10
        sw      (sp+44), r11
        sw      (sp+48), r12
        sw      (sp+52), r13
        sw      (sp+56), r14
        sw      (sp+60), r15
        sw      (sp+64), r16
        sw      (sp+68), r17
        sw      (sp+72), r18
        sw      (sp+76), r19
        sw      (sp+80), r20
        sw      (sp+84), r21
        sw      (sp+88), r22
        sw      (sp+92), r23
        sw      (sp+96), r24
        sw      (sp+100), r25
        sw      (sp+104), r26
        sw      (sp+108), r27
        /*          112   sp */
        /*          116   ra */
        sw      (sp+120), ea
        sw      (sp+124), ba
        /* Save interrupt enable */
        rcsr    r1, IE
        sw      (sp+128), r1
        /* ra and sp need special handling, as they have been modified */
        lw      r1, (sp+132)
        sw      (sp+116), r1
        mv      r1, sp
        addi    r1, r1, 132
        sw      (sp+112), r1
        ret

        /* Restore all registers and return from exception */
_restore_all_and_return:
        /* Restore interrupt enable */
        lw      r1, (sp+128)
        wcsr    IE, r1
        /* restore others */
        lw      r1, (sp+4)
        lw      r2, (sp+8)
        lw      r3, (sp+12)
        lw      r4, (sp+16)
        lw      r5, (sp+20)
        lw      r6, (sp+24)
        lw      r7, (sp+28)
        lw      r8, (sp+32)
        lw      r9, (sp+36)
        lw      r10, (sp+40)
        lw      r11, (sp+44)
        lw      r12, (sp+48)
        lw      r13, (sp+52)
        lw      r14, (sp+56)
        lw      r15, (sp+60)
        lw      r16, (sp+64)
        lw      r17, (sp+68)
        lw      r18, (sp+72)
        lw      r19, (sp+76)
        lw      r20, (sp+80)
        lw      r21, (sp+84)
        lw      r22, (sp+88)
        lw      r23, (sp+92)
        lw      r24, (sp+96)
        lw      r25, (sp+100)
        lw      r26, (sp+104)
        lw      r27, (sp+108)
        lw      ra, (sp+116)
        lw      ea, (sp+120)
        lw      ba, (sp+124)
        /* Stack pointer must be restored last, in case it has been updated */
        lw      sp, (sp+112)
        eret

