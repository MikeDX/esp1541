/**
 * Stub for rpi-gpio.h - provides Pi1541 GPIO API on ESP32
 * IEC bus: ATN=12, CLK=13, DATA=14, RESET=15
 */
#ifndef RPI_GPIO_STUB_H
#define RPI_GPIO_STUB_H

#include "esp32_gpio.h"

typedef int rpi_gpio_pin_t;

// ESP32 IEC bus pin mapping (non-split, 4 pins to level shifters)
enum PIGPIO_ESP32 {
    PIGPIO_ATN   = 12,
    PIGPIO_CLOCK = 13,
    PIGPIO_DATA  = 14,
    PIGPIO_RESET = 15,
    PIGPIO_OUT_LED   = 2,   // Built-in LED
    PIGPIO_OUT_SOUND = 4,   // Unused when SoundOnGPIO=false
};

// Rotary encoder pins (disabled on ESP32)
#define RPI_GPIO22  22
#define RPI_GPIO23  23
#define RPI_GPIO27  27

// GPIO function select (matches BCM2835)
#define FS_INPUT  0
#define FS_OUTPUT 1

static inline void RPI_SetGpioInput(rpi_gpio_pin_t pin)
{
    RPI_SetGpioPinFunction(pin, FS_INPUT);
}

// RPI_SetGpioPinFunction defined in esp32_gpio.h

// Stub GPPUD/GPPUDCLK0 - no-op on ESP32 (pull-ups configured per-pin if needed)
struct RPI_GpioBase_t {
    volatile uint32_t GPPUD;
    volatile uint32_t GPPUDCLK0;
};
static RPI_GpioBase_t _rpi_gpio_stub = {0, 0};
#define RPI_GpioBase (&_rpi_gpio_stub)

#endif /* RPI_GPIO_STUB_H */
