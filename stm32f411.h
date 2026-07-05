#pragma once

#include <stdint.h>

#define f_APB1  16000000U
#define BAUD      115200U

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830U)
#define RCC_AHB1ENR_GPIOAEN  (1U << 0)

#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840U)
#define RCC_APB1ENR_USART2EN  (1U << 17)

#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000U)
#define GPIOA_AFRL  (*(volatile uint32_t *)0x40020020U)

#define USART2_SR (*(volatile uint32_t *)0x40004400U)
#define USART2_DR (*(volatile uint32_t *)0x40004404U)
#define USART2_BRR (*(volatile uint32_t *)0x40004408U)
#define USART2_CR1 (*(volatile uint32_t *)0x4000440CU)
