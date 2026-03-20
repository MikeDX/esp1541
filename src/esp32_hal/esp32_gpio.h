/**
 * ESP32 HAL for Pi1541 - Raspberry Pi GPIO emulation
 * IEC bus pins: ATN=12, CLK=13, DATA=14, RESET=15 (to C64 via level shifters)
 */
#ifndef ESP32_GPIO_H
#define ESP32_GPIO_H

#include <stdint.h>
#include <driver/gpio.h>
#include <esp_timer.h>

// Token constants for Pi1541 compatibility (addr is ignored, we dispatch by constant)
#define ARM_GPIO_GPLEV0   0x7E200034
#define ARM_GPIO_GPCLR0   0x7E200028
#define ARM_GPIO_GPSET0   0x7E20001C
#define ARM_GPIO_GPFSEL1  0x7E200004
#define ARM_GPIO_GPPUD    0x7E200094
#define ARM_GPIO_GPPUDCLK0 0x7E200098

/* Must differ from GPFSEL1 — Pi uses 0x7E003004 for system timer CLO */
#define ARM_SYSTIMER_CLO  0x7E003004UL

// IEC bus pin assignments
#define ESP32_PIN_ATN     12
#define ESP32_PIN_CLK     13
#define ESP32_PIN_DATA    14
#define ESP32_PIN_RESET   15

// Pack IEC pins into a GPLEV0-style bitmask (bits 12-15)
#define ESP32_IEC_READ_MASK  ((1u << ESP32_PIN_ATN) | (1u << ESP32_PIN_CLK) | (1u << ESP32_PIN_DATA) | (1u << ESP32_PIN_RESET))

static inline uint32_t read32(uint32_t addr)
{
    if (addr == ARM_GPIO_GPLEV0) {
        uint32_t val = 0;
        if (gpio_get_level((gpio_num_t)ESP32_PIN_ATN))   val |= (1u << ESP32_PIN_ATN);
        if (gpio_get_level((gpio_num_t)ESP32_PIN_CLK))    val |= (1u << ESP32_PIN_CLK);
        if (gpio_get_level((gpio_num_t)ESP32_PIN_DATA))  val |= (1u << ESP32_PIN_DATA);
        if (gpio_get_level((gpio_num_t)ESP32_PIN_RESET)) val |= (1u << ESP32_PIN_RESET);
        return val;
    }
    if (addr == ARM_SYSTIMER_CLO) {
        return (uint32_t)(esp_timer_get_time());
    }
    return 0;
}

static inline void write32(uint32_t addr, uint32_t val)
{
    (void)val;
    if (addr == ARM_GPIO_GPPUD || addr == ARM_GPIO_GPPUDCLK0) {
        return;  /* No-op for pull-up/down on ESP32 */
    }
    if (addr == ARM_GPIO_GPCLR0) {
        if (val & (1u << ESP32_PIN_ATN))   gpio_set_level((gpio_num_t)ESP32_PIN_ATN, 0);
        if (val & (1u << 2))               gpio_set_level((gpio_num_t)2, 0);  /* LED */
        if (val & (1u << ESP32_PIN_CLK))    gpio_set_level((gpio_num_t)ESP32_PIN_CLK, 0);
        if (val & (1u << ESP32_PIN_DATA))   gpio_set_level((gpio_num_t)ESP32_PIN_DATA, 0);
        if (val & (1u << ESP32_PIN_RESET))  gpio_set_level((gpio_num_t)ESP32_PIN_RESET, 0);
        return;
    }
    if (addr == ARM_GPIO_GPSET0) {
        if (val & (1u << ESP32_PIN_ATN))   gpio_set_level((gpio_num_t)ESP32_PIN_ATN, 1);
        if (val & (1u << ESP32_PIN_CLK))   gpio_set_level((gpio_num_t)ESP32_PIN_CLK, 1);
        if (val & (1u << ESP32_PIN_DATA))  gpio_set_level((gpio_num_t)ESP32_PIN_DATA, 1);
        if (val & (1u << ESP32_PIN_RESET)) gpio_set_level((gpio_num_t)ESP32_PIN_RESET, 1);
        return;
    }
    /* Non-split IEC: Pi toggles GPFSEL1 so DATA/CLOCK are input (released) vs output (pull low) */
    if (addr == ARM_GPIO_GPFSEL1) {
        const unsigned clk_pin = ESP32_PIN_CLK;
        const unsigned data_pin = ESP32_PIN_DATA;
        unsigned clk_fn = (val >> ((clk_pin - 10u) * 3u)) & 7u;
        unsigned data_fn = (val >> ((data_pin - 10u) * 3u)) & 7u;
        gpio_num_t gclk = (gpio_num_t)clk_pin;
        gpio_num_t gdata = (gpio_num_t)data_pin;
        if (clk_fn == 1u) {
            gpio_set_direction(gclk, GPIO_MODE_OUTPUT_OD);
            gpio_set_level(gclk, 0);
        } else {
            gpio_set_direction(gclk, GPIO_MODE_INPUT);
            gpio_set_pull_mode(gclk, GPIO_FLOATING);
        }
        if (data_fn == 1u) {
            gpio_set_direction(gdata, GPIO_MODE_OUTPUT_OD);
            gpio_set_level(gdata, 0);
        } else {
            gpio_set_direction(gdata, GPIO_MODE_INPUT);
            gpio_set_pull_mode(gdata, GPIO_FLOATING);
        }
        return;
    }
}

static inline void RPI_SetGpioPinFunction(int pin, int mode)
{
    gpio_num_t gpio = (gpio_num_t)pin;
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << pin);
    cfg.mode = (mode == 1) ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;  // FS_OUTPUT=1, FS_INPUT=0
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);
}

#endif /* ESP32_GPIO_H */
