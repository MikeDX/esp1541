#include "ram_rom.h"
#include <Arduino.h>
#include <esp_heap_caps.h>
#include <string.h>

#ifndef MALLOC_CAP_SPIRAM
#define MALLOC_CAP_SPIRAM  (1 << 3)
#endif

static uint8_t* rom_data = nullptr;
static bool loaded = false;

bool ram_rom_init(void) {
  if (rom_data) return true;
  rom_data = (uint8_t*)heap_caps_malloc(RAM_ROM_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!rom_data) rom_data = (uint8_t*)malloc(RAM_ROM_SIZE);
  if (rom_data) {
    memset(rom_data, 0, RAM_ROM_SIZE);
    loaded = false;
    return true;
  }
  return false;
}

bool ram_rom_load(const uint8_t* data, size_t size) {
  if (!rom_data || !data || size != RAM_ROM_SIZE) return false;
  memcpy(rom_data, data, RAM_ROM_SIZE);
  loaded = true;
  return true;
}

bool ram_rom_loaded(void) {
  return loaded && rom_data;
}

uint8_t* ram_rom_get(void) {
  return rom_data;
}
