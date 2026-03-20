/**
 * RAM disk storage - holds D64/G64 images in PSRAM
 */

#ifndef RAM_DISK_H
#define RAM_DISK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define RAM_DISK_MAX_SLOTS     4
#define RAM_DISK_MAX_SIZE     (512 * 1024)   // 512KB per slot (enough for G64)
#define RAM_DISK_TOTAL_SIZE   (RAM_DISK_MAX_SLOTS * RAM_DISK_MAX_SIZE)

typedef enum {
  RAM_DISK_TYPE_NONE = 0,
  RAM_DISK_TYPE_D64,
  RAM_DISK_TYPE_G64,
  RAM_DISK_TYPE_NIB,
  RAM_DISK_TYPE_UNKNOWN
} ram_disk_type_t;

typedef struct {
  uint8_t*  data;
  size_t    size;
  ram_disk_type_t type;
  char      filename[64];
  bool      loaded;
} ram_disk_slot_t;

void ram_disk_init(void);
void ram_disk_clear_slot(int slot);
bool ram_disk_load(int slot, const uint8_t* data, size_t size, const char* filename);
ram_disk_slot_t* ram_disk_get_slot(int slot);
int ram_disk_find_free_slot(void);
size_t ram_disk_free_bytes(void);

#endif
