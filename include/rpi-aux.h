/**
 * Stub for rpi-aux.h on ESP32
 */
#ifndef RPI_AUX_H
#define RPI_AUX_H

#ifdef __ESP32__
#include <esp_timer.h>
static inline void usDelay(unsigned nMicroSeconds) {
  int64_t end = esp_timer_get_time() + (int64_t)nMicroSeconds;
  while (esp_timer_get_time() < end) {}
}
#endif

#endif
