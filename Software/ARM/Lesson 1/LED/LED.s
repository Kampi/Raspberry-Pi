@-------------------------------------------------------------------------
@
@ Copyright (c) 2018 Daniel Kampert kampis-elektroecke.com
@
@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
@ to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
@ and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
@
@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
@
@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
@ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
@ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
@ IN THE SOFTWARE.
@
@-------------------------------------------------------------------------

# @file main.s
# @brief Example programm for JTAG debugging.
#        This programm activate the Act-LED on the raspberry pi.
#
# Please check the article for more information
# https://www.kampis-elektroecke.de/raspberry-pi/raspberry-pi-assembler/lektion-2/
#
# @author Daniel Kampert

.section .init
    .globl _start

.section .text
    _start:

    @ Base adress for gpio controller
    ldr r0, =0x20200000

    @ Set GPIO47 as output
    ldr r1, [r0, #0x10]
    mov r2, #0x07
    lsl r2, #0x15
    bic r1, r2
    mov r2, #0x01
    lsl r2, #0x15
    orr r1, r2
    str r1, [r0, #0x10]

    @ Clear GPIO47
    mov r1, #0x01
    lsl r1, #0x0F
    str r1, [r0, #0x20]

    loop$:
    @ Jump to label "loop"
    b loop$
