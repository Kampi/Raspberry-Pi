#-------------------------------------------------------------------------
#
# Copyright (c) 2018 Daniel Kampert kampis-elektroecke.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
# to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
# IN THE SOFTWARE.
#
#-------------------------------------------------------------------------

# @file main.s
# @brief JTAG debugging example for the raspberry pi.
#        This programm switch all needed pins on P1 in the JTAG mode.
#
# Please check the article for more information
# https://www.kampis-elektroecke.de/raspberry-pi/raspberry-pi-assembler/lektion-2/
#
# @author Daniel Kampert

# ARM_TRST	P1-15	(22 ALT4)
# ARM_TDO	P1-18	(24 ALT4)
# ARM_TCK	P1-22	(25 ALT4)
# ARM_TDI	P1-7 	( 4 ALT5)
# ARM_TMS	P1-13	(27 ALT4)
# ARM_RTCK	P1-16	(27 ALT4)

.section .init
.align 4
    .globl _start

    _start:

    @ Clear r3 and r4
    mov r3, #0x00
    mov r4, #0x00

    @ Base adress for gpio controller
    ldr r0, =0x20200000

    @ Store "0x02" for ALT5
    mov r1, #0x02

    @ Store "0x03" for ALT4
    mov r2, #0x03

    @ ARM_TDI
    @ Copy the value for ALT5
    @ Shift it 12 places to access gpio 4 (Bit 14 to 12)
    @ Copy the value to the register r0 + 0 (GPFSEL0)
    mov r3, r1
    lsl r3, #0x0C
    str r3, [r0, #0x00]

    @ ARM_TRST
    @ Copy the value for ALT4
    @ Shift it 6 places to access gpio 22 (Bit 8 to 6)
    mov r3, r2
    lsl r3, #0x06
    orr r4, r3

    @ ARM_RTCK
    @ Copy the value for ALT4
    @ Shift it 9 places to access gpio 23 (Bit 11 to 9)
    mov r3, r2
    lsl r3, #0x09
    orr r4, r3

    @ ARM_TDO
    @ Copy the value for ALT4
    @ Shift it 12 places to access gpio 24 (Bit 14 to 12)
    mov r3, r2
    lsl r3, #0x0C
    orr r4, r3

    @ ARM_TCK
    @ Copy the value for ALT4
    @ Shift it 15 places to access gpio 25 (Bit 17 to 15)
    mov r3, r2
    lsl r3, #0x0F
    orr r4, r3

    @ ARM_TMS
    @ Copy the value for ALT4
    @ Shift it 21 places to access gpio 27 (Bit 23 to 21)
    mov r3, r2
    lsl r3, #0x15
    orr r4, r3

    @ Copy the value to the register r0 + 8 (GPFSEL2)
    str r4, [r0, #0x08]

    loop$:
    @ Jump to label "loop"
    b loop$
