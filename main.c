#include <stdint.h>
#include "stm32f411.h"

#define TRIG_HIGH()  (GPIOA_ODR |=  (1U << 6))
#define TRIG_LOW()   (GPIOA_ODR &= ~(1U << 6))
#define ECHO_HIGH    (GPIOA_IDR &   (1U << 7))

#define ECHO_TIMEOUT_US  30000U
#define MEASURE_PAUSE_US 60000U

void write_byte(uint8_t b) {
    while (!(USART2_SR & (1U << 7))) {}
    USART2_DR = b;
}

void print_string(const char *s) {
    while (*s) write_byte(*s++);
}

void write_uint32(uint32_t val) {
    if (val == 0) {
        write_byte('0');
        return;
    }
    char buf[10];
    int i = 0;
    while (val > 0) {
        buf[i++] = val % 10 + '0';
        val /= 10;
    }
    while (i > 0) {
        write_byte(buf[--i]);
    }
}

void write_float(float num, uint8_t precision) {
    if (num < 0.0f) {
        write_byte('-');
        num = -num;
    }

    float rounding = 0.5f;
    for (uint8_t i = 0; i < precision; i++) {
        rounding /= 10.0f;
    }
    num += rounding;

    uint32_t int_part = (uint32_t)num;
    float frac_part = num - (float)int_part;

    write_uint32(int_part);
    if (precision > 0) {
        write_byte('.');
        for (uint8_t i = 0; i < precision; i++) {
            frac_part *= 10.0f;
            uint8_t digit = (uint8_t)frac_part;
            write_byte(digit + '0');
            frac_part -= (float)digit;
        }
    }
}

static void delay_us(uint32_t us) {
    TIM2_CNT = 0;
    while (TIM2_CNT < us) {}
}

static uint32_t measure_echo_us(void) {
    TRIG_HIGH();
    delay_us(10);
    TRIG_LOW();

    TIM2_CNT = 0;
    while (!ECHO_HIGH) {
        if (TIM2_CNT > ECHO_TIMEOUT_US) return 0;
    }

    TIM2_CNT = 0;
    while (ECHO_HIGH) {
        if (TIM2_CNT > ECHO_TIMEOUT_US) return 0;
    }
    return TIM2_CNT;
}

static void setup(void) {
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM2EN;

    GPIOA_MODER &= ~(3U << 4);
    GPIOA_MODER |=  (1U << 5);
    GPIOA_MODER &= ~(3U << 6);
    GPIOA_MODER |=  (1U << 7);
    GPIOA_AFRL  &= ~(15U << 8);
    GPIOA_AFRL  |=  (7U << 8);
    GPIOA_AFRL  &= ~(15U << 12);
    GPIOA_AFRL  |=  (7U << 12);

    GPIOA_MODER &= ~(3U << 12);
    GPIOA_MODER |=  (1U << 12);
    GPIOA_MODER &= ~(3U << 14);

    USART2_BRR = (f_APB1 + BAUD / 2) / BAUD;
    USART2_CR1 |= (1U << 3) | (1U << 13);

    TIM2_PSC = 16U - 1U;
    TIM2_ARR = 0xFFFFFFFFU;
    TIM2_EGR = (1U << 0);
    TIM2_CR1 |= (1U << 0);
    
    TRIG_LOW();
}

int main(void) {
    setup();
    print_string("HC-SR04\r\n");

    while (1) {
        uint32_t echo_us = measure_echo_us();

        if (echo_us == 0) {
            print_string("No echo\r\n");
        } else {
            print_string("Distance: ");
            write_float(echo_us / 58.0f, 1);
            print_string(" cm\r\n");
        }

        delay_us(MEASURE_PAUSE_US);
    }
}