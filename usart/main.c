#include <stdint.h>

#define RCC_AHBENR   (*(volatile uint32_t*)0x40021014)
#define RCC_APB1ENR  (*(volatile uint32_t*)0x4002101C)

#define GPIOA_MODER  (*(volatile uint32_t*)0x48000000)
#define GPIOA_AFRL   (*(volatile uint32_t*)0x48000020)
#define GPIOA_ODR    (*(volatile uint32_t*)0x48000014)

#define USART2_CR1   (*(volatile uint32_t*)0x40004400)
#define USART2_BRR   (*(volatile uint32_t*)0x4000440C)
#define USART2_ISR   (*(volatile uint32_t*)0x4000441C)
#define USART2_RDR   (*(volatile uint32_t*)0x40004424)
#define USART2_TDR   (*(volatile uint32_t*)0x40004428)

#define PIN_LED_1 6

void delay(volatile uint32_t t) {
  while (t--) __asm__("nop");
}

void turnOnLed() {
  GPIOA_ODR |= (1 << PIN_LED_1);
}

void turnOffLed() {
  GPIOA_ODR &= ~(1 << PIN_LED_1);
}

void uart_init(void) {
  RCC_AHBENR  |= (1 << 17);
  RCC_APB1ENR |= (1 << 17);

  GPIOA_MODER &= ~((0b11 << (2 * 2)) | (0b11 << (3 * 2)));
  GPIOA_MODER |=  (0b10 << (2 * 2)) | (0b10 << (3 * 2));
  GPIOA_AFRL  &= ~((0xF << (4 * 2)) | (0xF << (4 * 3)));
  GPIOA_AFRL  |=  (0x7 << (4 * 2)) | (0x7 << (4 * 3));

 // 8 МГц -> 9600 = 8000000 / 9600 = ~833
  USART2_BRR = 833;

  USART2_CR1 = (1 << 2) | (1 << 3) | (1 << 0);
}

char uart_receive(void) {
  while (!(USART2_ISR & (1 << 5))) ;
  return (char)USART2_RDR;
}

void uart_send(char c) {
  while (!(USART2_ISR & (1 << 7))) ;
  USART2_TDR = c;
}

void uart_send_str(const char* s) {
  while (*s) uart_send(*s++);
}

int main(void) {
  RCC_AHBENR |= (1 << 17);

  GPIOA_MODER &= ~(0b11 << (PIN_LED_1 * 2));
  GPIOA_MODER |=  (0b01 << (PIN_LED_1 * 2));

  uart_init();

  uart_send_str("Terminal ready\r\n");

  while (1) {
    char c = uart_receive();

    if (c == '1') {
      turnOnLed();
      uart_send_str("LED ON\r\n");
    } else if (c == '0') {
      turnOffLed();
      uart_send_str("LED OFF\r\n");
    } else {
      uart_send_str("Unknown command\r\n");
    }
  }
}
