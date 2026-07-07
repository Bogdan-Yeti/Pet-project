#include <stdint.h>
#include "stm32f411.h"

#define BTN_PRESSED  (!(GPIOC_IDR & (1U << 13))) 
#define LED_ON()     (GPIOA_ODR |=  (1U << 5))
#define LED_OFF()    (GPIOA_ODR &= ~(1U << 5))
#define TIM2_ON()     (TIM2_CR1 |= (1U << 0))
#define TIM2_OFF()    (TIM2_CR1 &= ~(1U << 0))
#define TIM2_UPD()    (TIM2_EGR = (1U << 0))

void TIM2_IRQHandler(void) {
    LED_OFF();
    TIM2_DIER &= ~(1U << 0);
    TIM2_SR &= ~(1U << 0); 
    TIM2_CNT = 0;
}

void write_byte(uint8_t b) {
    while (!(USART2_SR & (1U << 7))) {}
    USART2_DR = b;
}

void print_number(uint32_t num) {
    uint8_t buf[11];
    uint8_t i = 11;

    if (num == 0) {
        write_byte('0');
        return;
    }
    while (num) {
        buf[--i] = num % 10 + 48;
        num /= 10;
        if (i == 8) buf[--i] = 44;
    }
    if (i == 7) buf[--i] = 48;

    while (i < 11) {
        write_byte(buf[i++]);
    }
}

void print_string(char *b) {
    while (*b) write_byte(*b++);
}

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    RCC_APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOA_MODER &= ~(3U << 4);
    GPIOA_MODER &= ~(3U << 6);

    GPIOA_MODER |= (1U << 5);
    GPIOA_MODER |= (1U << 7);

    /*Output mode on PA5 (LED2)*/
    GPIOA_MODER &= ~(3U << 10);
    GPIOA_MODER |= (1U << 10);

    GPIOC_MODER &= ~(3U << 26);

    GPIOA_AFRL &= ~(15U << 8);
    GPIOA_AFRL &= ~(15U << 12);

    GPIOA_AFRL |= (7U << 8);
    GPIOA_AFRL |= (7U << 12);

    USART2_BRR = ((f_APB1 + BAUD/2) / BAUD);

    USART2_CR1 |= (1U << 3);
    USART2_CR1 |= (1U << 13);

    TIM2_PSC = (16000U - 1U);
    TIM2_ARR = (5000U);
    TIM2_UPD();
    TIM2_SR &= ~(1U << 0);
    TIM2_DIER |= (1U << 0);

    NVIC_ISER0 |= (1U << 28);
    print_string("Press button to start\r\n");
    while (1) {
        if (BTN_PRESSED) {
            if (!(TIM2_CR1 & (1U << 0))) {
                LED_ON();
                TIM2_ON();
            } else if (!(GPIOA_ODR & (1U << 5))) {
                print_number(TIM2_CNT);
                write_byte('\r');
                write_byte('\n');
                TIM2_OFF();
                TIM2_DIER |= (1U << 0);
                while (BTN_PRESSED) {}
                print_string("Press button to start\r\n");
            }
        }
    }
}
