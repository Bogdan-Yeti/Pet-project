.syntax unified         @ Использовать современный синтаксис UAL
    .thumb                  @ Генерировать 16/32-битные инструкции Thumb
    
    .text
    .global Reset_Handler
    .type Reset_Handler, %function  @ ВАЖНО: Указывает линкеру, что это Thumb-код (ставит младший бит адреса в 1)
Reset_Handler:
    @ Инициализация секции .data (копирование из Flash в RAM)
    ldr     r1, =_sidata    @ r1 - источник (аналог %esi)
    ldr     r2, =_sdata     @ r2 - приемник (аналог %edi)
    ldr     r3, =_edata     @ r3 - конец данных
    b       .Lcopy_test     @ Безусловный переход (вместо jmp)

.Lcopy_body:
    ldr     r0, [r1]        @ Читаем слово по адресу r1 (аналог movl (%esi), %eax)
    str     r0, [r2]        @ Пишем слово по адресу r2 (аналог movl %eax, (%edi))
    adds    r1, r1, #4      @ Сдвигаем указатель источника
    adds    r2, r2, #4      @ Сдвигаем указатель приемника

.Lcopy_test:
    cmp     r2, r3          @ Сравниваем текущий адрес приемника с концом секции
    blo     .Lcopy_body     @ Branch if Lower (беззнаковое меньше, аналог jb)

    @ Инициализация секции .bss (заполнение нулями)
    ldr     r2, =_sbss      @ Указатель на начало BSS
    ldr     r3, =_ebss      @ Указатель на конец BSS
    movs    r0, #0          @ Подготавливаем ноль для записи
    b       .Lzero_test

.Lzero_body:
    str     r0, [r2]        @ Записываем ноль
    adds    r2, r2, #4      @ Сдвигаем указатель

.Lzero_test:
    cmp     r2, r3          @ Дошли до конца BSS?
    blo     .Lzero_body     @ Если нет, продолжаем

    @ Переход к основной программе
    bl      main            @ Branch with Link (вызов функции, аналог call)

.Ltrap:
    b       .Ltrap          @ Бесконечный цикл, если main вернет управление

    .global Default_Handler
    .type Default_Handler, %function
Default_Handler:
.Lan_hang:
    b       .Lan_hang       @ Заглушка для неиспользуемых прерываний

    @ Таблица векторов прерываний
    .section .isr_vector, "a", %progbits
    .align  2               @ В ARM выравнивание задается степенью двойки (2^2 = 4 байта)
    .global vector_table
vector_table:
    .word   _estack         @ .word в ARM GAS - это 32-битное слово (аналог .long)
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