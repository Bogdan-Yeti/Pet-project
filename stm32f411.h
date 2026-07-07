#pragma once

#include <stdint.h>

#define f_APB1  16000000U
#define BAUD      115200U

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830U)
#define RCC_AHB1ENR_GPIOAEN  (1U << 0)
#define RCC_AHB1ENR_GPIOCEN  (1U << 2)

#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840U)
#define RCC_APB1ENR_USART2EN  (1U << 17)
#define RCC_APB1ENR_TIM2EN  (1U << 0)

#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000U)
#define GPIOA_ODR  (*(volatile uint32_t *)0x40020014U)
#define GPIOA_AFRL  (*(volatile uint32_t *)0x40020020U)

#define GPIOC_MODER  (*(volatile uint32_t *)0x40020800U)
#define GPIOC_IDR  (*(volatile uint32_t *)0x40020810U)
#define GPIOC_LCKR  (*(volatile uint32_t *)0x4002081CU)

#define USART2_SR (*(volatile uint32_t *)0x40004400U)
#define USART2_DR (*(volatile uint32_t *)0x40004404U)
#define USART2_BRR (*(volatile uint32_t *)0x40004408U)
#define USART2_CR1 (*(volatile uint32_t *)0x4000440CU)

#define TIM2_CR1 (*(volatile uint32_t *)0x40000000U)
#define TIM2_EGR  (*(volatile uint32_t *)0x40000014U)
#define TIM2_CNT (*(volatile uint32_t *)0x40000024U)
#define TIM2_PSC (*(volatile uint32_t *)0x40000028U)
#define TIM2_ARR (*(volatile uint32_t *)0x4000002CU)
#define TIM2_DIER  (*(volatile uint32_t *)0x4000000CU)
#define TIM2_SR  (*(volatile uint32_t *)0x40000010U)

#define NVIC_ISER0  (*(volatile uint32_t *)0xE000E100U)