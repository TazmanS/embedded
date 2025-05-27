#include <stdint.h>

// RCC
#define RCC_AHBENR    (*(volatile uint32_t*)0x40021014)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x4002101C)

// GPIOA и GPIOB
#define GPIOA_MODER   (*(volatile uint32_t*)0x48000000)
#define GPIOA_AFRL    (*(volatile uint32_t*)0x48000020)
#define GPIOB_MODER   (*(volatile uint32_t*)0x48000400)
#define GPIOB_AFRL    (*(volatile uint32_t*)0x48000420)

// TIM3
#define TIM3_CR1      (*(volatile uint32_t*)0x40000400)
#define TIM3_CCMR1    (*(volatile uint32_t*)0x40000418)
#define TIM3_CCMR2    (*(volatile uint32_t*)0x4000041C)
#define TIM3_CCER     (*(volatile uint32_t*)0x40000420)
#define TIM3_PSC      (*(volatile uint32_t*)0x40000428)
#define TIM3_ARR      (*(volatile uint32_t*)0x4000042C)
#define TIM3_CCR1     (*(volatile uint32_t*)0x40000434)
#define TIM3_CCR2     (*(volatile uint32_t*)0x40000438)
#define TIM3_CCR3     (*(volatile uint32_t*)0x4000043C)
#define TIM3_EGR      (*(volatile uint32_t*)0x40000414)

void delay(volatile uint32_t t) {
  while (t--) __asm__("nop");
}

int rbg(void) {
  RCC_AHBENR  |= (1 << 17) | (1 << 18);
  RCC_APB1ENR |= (1 << 1);

  GPIOA_MODER &= ~((0b11 << (6 * 2)) | (0b11 << (7 * 2)));
  GPIOA_MODER |=  ((0b10 << (6 * 2)) | (0b10 << (7 * 2)));
  GPIOA_AFRL  &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
  GPIOA_AFRL  |=  ((0x2 << (6 * 4)) | (0x2 << (7 * 4)));

  GPIOB_MODER &= ~(0b11 << (0 * 2));
  GPIOB_MODER |=  (0b10 << (0 * 2));
  GPIOB_AFRL  &= ~(0xF << (0 * 4));
  GPIOB_AFRL  |=  (0x2 << (0 * 4));

  TIM3_PSC  = 72 - 1;
  TIM3_ARR  = 1000 - 1;

  TIM3_CCR1 = 0;
  TIM3_CCR2 = 0;
  TIM3_CCR3 = 0;

  TIM3_CCMR1 |= (6 << 4) | (1 << 3);
  TIM3_CCMR1 |= (6 << 12) | (1 << 11);
  TIM3_CCMR2 |= (6 << 4) | (1 << 3);

  TIM3_CCER |= (1 << 0);
  TIM3_CCER |= (1 << 4);
  TIM3_CCER |= (1 << 8);

  TIM3_EGR  |= 1;
  TIM3_CR1  |= 1;

  while (1) {
    for (uint16_t i = 0; i < 1000; i++) {
      TIM3_CCR1 = i;
      TIM3_CCR2 = 1000 - i;
      TIM3_CCR3 = i / 2;
      delay(5000);
    }
  }
}
