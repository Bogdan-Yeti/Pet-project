.syntax unified
    .thumb
    
    .text 
    .global Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    ldr     r1, =_sidata
    ldr     r2, =_sdata
    ldr     r3, =_edata
    b       .Lcopy_test

.Lcopy_body:
    ldr     r0, [r1]
    str     r0, [r2]
    adds    r1, r1, #4
    adds    r2, r2, #4

.Lcopy_test:
    cmp     r2, r3
    blo     .Lcopy_body

    ldr     r2, =_sbss
    ldr     r3, =_ebss
    movs    r0, #0
    b       .Lzero_test

.Lzero_body:
    str     r0, [r2]
    adds    r2, r2, #4

.Lzero_test:
    cmp     r2, r3
    blo     .Lzero_body

    bl      main

.Ltrap:
    b       .Ltrap

    .global Default_Handler
    .type Default_Handler, %function
Default_Handler:
.Lan_hang:
    b       .Lan_hang

    .section .isr_vector, "a", %progbits
    .align  2
    .global vector_table
vector_table:
    .word   _estack
    .word   Reset_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   0, 0, 0, 0
    .word   Default_Handler
    .word   Default_Handler
    .word   0
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   Default_Handler
    .word   TIM2_IRQHandler
