/* Stub for rpi-i2c.h on ESP32 - no I2C for SSD1306 in minimal build */
#ifndef RPI_I2C_H
#define RPI_I2C_H
#ifdef __ESP32__
static inline void RPI_I2CInit(int bus, int baud) {
    (void)bus; (void)baud;
}
static inline void RPI_I2CWrite(int bus, int addr, const void* buf, int len) {
    (void)bus; (void)addr; (void)buf; (void)len;
}
#endif
#endif
