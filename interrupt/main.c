#include <stdint.h>

#define RCC_AHBENR (*(volatile uint32_t *)0x40021014)  // Clock control register for GPIOA
#define GPIOA_MODER (*(volatile uint32_t *)0x48000000) // GPIOA pin configuration register
#define GPIOA_IDR (*(volatile uint32_t *)0x48000010)   // GPIOA input data register
#define GPIOA_ODR (*(volatile uint32_t *)0x48000014)   // GPIOA output data register

#define EXTI_IMR (*(volatile uint32_t *)0x40010400)   // EXTI interrupt mask register
#define EXTI_FTSR (*(volatile uint32_t *)0x4001040C)  // EXTI falling trigger selection register
#define EXTI_PR (*(volatile uint32_t *)0x40010414)    // EXTI pending register
#define NVIC_ISER0 (*(volatile uint32_t *)0xE000E100) // NVIC interrupt set-enable register 0

#define LED_PIN 5    // LED pin (PA5)
#define BUTTON_PIN 0 // Button pin (PA0)

// Delay function
void delay(volatile uint32_t t)
{
  while (t--)
    __asm__("nop"); // No operation (NOP) for delay
}

// EXTI configuration for PA0
void EXTI_Config(void)
{
  // Enable clock for GPIOA
  RCC_AHBENR |= (1 << 17);

  // Set PA0 as input with pull-up resistor
  GPIOA_MODER &= ~(0b11 << (BUTTON_PIN * 2)); // Set PA0 as input

  // Enable internal pull-up resistor for PA0
  GPIOA_IDR |= (1 << BUTTON_PIN); // Enable pull-up (will pull to HIGH)

  // Enable interrupt for PA0 (EXTI0)
  EXTI_IMR |= (1 << BUTTON_PIN);  // Enable interrupt for EXTI0
  EXTI_FTSR |= (1 << BUTTON_PIN); // Falling edge trigger (when button is pressed)

  // Enable interrupt in NVIC
  NVIC_ISER0 = (1 << 6); // Enable interrupt handler for EXTI0 (interrupt number 6)
}

// Interrupt handler for EXTI0 (PA0)
void EXTI0_IRQHandler(void)
{
  // Check if interrupt flag is set for PA0
  if (EXTI_PR & (1 << BUTTON_PIN))
  {
    EXTI_PR |= (1 << BUTTON_PIN); // Clear the interrupt flag

    // Turn off the LED (PA5 = LOW)
    GPIOA_ODR &= ~(1 << LED_PIN);

    // For debugging: you can add a delay here
    delay(100); // Delay for stabilization
  }
}

int main(void)
{
  // Enable clock for GPIOA
  RCC_AHBENR |= (1 << 17);

  // Set PA5 as output for LED
  GPIOA_MODER &= ~(0b11 << (LED_PIN * 2)); // Clear bits for PA5
  GPIOA_MODER |= (0b01 << (LED_PIN * 2));  // Set PA5 as output

  // Configure interrupt for PA0
  EXTI_Config();

  while (1)
  {
    // Turn on the LED (PA5 = HIGH) by default
    GPIOA_ODR |= (1 << LED_PIN);

    delay(1000000); // 1 second delay
  }

  return 0;
}
