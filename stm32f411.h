#pragma once

#include <stdint.h>

/* RCC — включает/выключает тактирование периферии (RM0383 §6.3) */
#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830U)  /* тактирование портов GPIO */
#define RCC_AHB1ENR_GPIOAEN  (1U << 0)  /* бит 0: включить порт A   */
#define RCC_AHB1ENR_GPIOCEN  (1U << 2)  /* бит 2: включить порт C   */

/* GPIOA, база 0x40020000 (RM0383 §8.4) */
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000U)  /* режим вывода: вход/выход/... */
#define GPIOA_ODR    (*(volatile uint32_t *)0x40020014U)  /* состояние выходов порта      */

/* GPIOC, база 0x40020800 (RM0383 §8.4) */
#define GPIOC_MODER (*(volatile uint32_t *)0x40020800U) /* режим вывода: вход/выход/... */
#define GPIOC_IDR  (*(volatile uint32_t *)0x40020810U)  /* Input Data Register */