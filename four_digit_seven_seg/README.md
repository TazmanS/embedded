# Four-Digit Seven Segment Display for STM32

This project provides low-level control of a four-digit seven-segment display using an STM32 microcontroller. It directly manipulates GPIO registers to display digits on the display without relying on external libraries or HAL.

## Features

* Direct register access (no HAL or LL drivers)
* Multiplexed display of 4 digits
* Segment mapping for digits 0–9
* Simple delay-based refreshing

## Wiring

* **Segments (A–G, DP)** are connected to **GPIOA pins**:

  * A: PA0
  * B: PA1
  * C: PA9
  * D: PA8
  * E: PA4
  * F: PA5
  * G: PA6
  * DP: PA7

* **Digit control lines (common cathodes or anodes)** are connected to **GPIOB pins**:

  * Digit 1: PB0
  * Digit 2: PB1
  * Digit 3: PB2
  * Digit 4: PB3

## Functions

### `void initFourDigitSevenSeg()`

Initializes GPIOA and GPIOB pins as output for segments and digit control.

### `void showDigits(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)`

Displays four digits (0–9) on the display using multiplexing.

## Example Usage

```c
int main() {
  initFourDigitSevenSeg();
  while (1) {
    showDigits(1, 2, 3, 4); // Display 1234
  }
}
```

## Notes

* The display is refreshed by cycling through digits with a short delay (`2 ms`) to give the illusion of continuous display (persistence of vision).
* Timing may need tuning depending on your system clock.

## License

This project is open source and free to use under the MIT License.

