#ifndef RAM_ROM_H
#define RAM_ROM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define RAM_ROM_SIZE 16384  // 1541 ROM size

bool ram_rom_init(void);
bool ram_rom_load(const uint8_t* data, size_t size);
bool ram_rom_loaded(void);
uint8_t* ram_rom_get(void);

#endif
