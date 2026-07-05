#include <stdint.h>
#include "stm32f411.h"

void write_byte(uint8_t b) {
    while (!(USART2_SR & (1U << 7))) {}
    USART2_DR = b;
}

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    RCC_APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA_MODER &= ~(3U << 4);
    GPIOA_MODER &= ~(3U << 6);

    GPIOA_MODER |= (1U << 5);
    GPIOA_MODER |= (1U << 7);

    GPIOC_MODER &= ~(3U << 26);

    GPIOA_AFRL &= ~(15U << 8);
    GPIOA_AFRL &= ~(15U << 12);

    GPIOA_AFRL |= (7U << 8);
    GPIOA_AFRL |= (7U << 12);

    USART2_BRR = ((f_APB1 + BAUD/2) / BAUD);

    USART2_CR1 |= (1U << 3);
    USART2_CR1 |= (1U << 13);

    uint8_t flag = 1;
    while (1) {
        if (!(GPIOC_IDR & (1U << 13)) && flag) {
            char *str = "Hello world\r\n\0";
            while (*str) { 
                write_byte(*str); 
                str++; 
            }
            flag = 0;
        } else if (GPIOC_IDR & (1U << 13)) {
            flag = 1;
        }
    }
}
