#include <stdint.h>
#include "stm32f411.h"

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOA_MODER &= ~(3U << 10);
    GPIOA_MODER |=  (1U << 10);

    GPIOC_MODER &= ~(3U << 26);

    while (1) {
        if ((GPIOC_IDR & (1U << 13)) == 0) {
            GPIOA_ODR |=  (1U << 5);
        } else {
            GPIOA_ODR &= ~(1U << 5);
        }
    }
}
