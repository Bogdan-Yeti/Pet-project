/* ============================================================
   startup.c — то, что выполняется СРАЗУ после включения,
   ещё до твоего main(). Тоже "чёрная коробка".

   Делает три вещи:
     1) копирует начальные значения переменных из flash в RAM;
     2) обнуляет переменные, которые должны стартовать с нуля;
     3) вызывает main().
   А в самом начале файла лежит "таблица векторов" — список
   адресов, куда процессор прыгает при старте и при прерываниях.
   ============================================================ */

#include <stdint.h>

/* Эти символы создаёт линкер (см. linker.ld) */
extern uint32_t _sidata;   /* адрес начальных значений .data во flash */
extern uint32_t _sdata;    /* начало .data в RAM                      */
extern uint32_t _edata;    /* конец  .data в RAM                      */
extern uint32_t _sbss;     /* начало .bss  в RAM                      */
extern uint32_t _ebss;     /* конец  .bss  в RAM                      */
extern uint32_t _estack;   /* вершина стека (используем её адрес)     */

int  main(void);           /* твоя программа */

/* --- Самая первая функция после reset --- */
void Reset_Handler(void)
{
    /* 1) Копируем инициализированные переменные из flash в RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* 2) Обнуляем .bss */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* 3) Передаём управление твоему коду */
    main();

    /* main() возвращаться не должен; если вдруг — крутимся на месте */
    while (1) { }
}

/* Заглушка для любого прерывания, обработчик которого мы ещё не написали */
void Default_Handler(void)
{
    while (1) { }
}

/* "Слабые" имена: пока ты не напишешь свою функцию с таким же именем,
   каждое прерывание ведёт в Default_Handler. Позже, например, ты
   определишь SysTick_Handler — и таблица сама начнёт звать его.        */
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

typedef void (*vector_t)(void);

/* Таблица векторов: первые 16 ячеек — системные.
   [0] = стартовое значение стека, [1] = откуда начать выполнение.   */
__attribute__((section(".isr_vector")))
const vector_t vector_table[] = {
    (vector_t)(&_estack),   /*  0  начальный указатель стека */
    Reset_Handler,          /*  1  старт                     */
    NMI_Handler,            /*  2                             */
    HardFault_Handler,      /*  3                             */
    MemManage_Handler,      /*  4                             */
    BusFault_Handler,       /*  5                             */
    UsageFault_Handler,     /*  6                             */
    0, 0, 0, 0,             /*  7-10  зарезервировано        */
    SVC_Handler,            /* 11                             */
    DebugMon_Handler,       /* 12                             */
    0,                      /* 13  зарезервировано           */
    PendSV_Handler,         /* 14                             */
    SysTick_Handler,        /* 15                             */
    /* прерывания периферии (UART, кнопка...) добавим здесь позже */
};
