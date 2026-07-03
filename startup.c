#include <stdint.h>

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

int  main(void);

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();

    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}

void NMI_Handler(void)        __attribute__((alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((alias("Default_Handler")));

typedef void (*vector_t)(void);

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
};
