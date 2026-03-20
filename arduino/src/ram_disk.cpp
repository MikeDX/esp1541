#include "ram_disk.h"
#include <Arduino.h>
#include <esp_heap_caps.h>
#include <string.h>

#ifndef MALLOC_CAP_SPIRAM
#define MALLOC_CAP_SPIRAM  (1 << 3)
#endif

static ram_disk_slot_t slots[RAM_DISK_MAX_SLOTS];
static bool initialized = false;

static ram_disk_type_t detect_type(const uint8_t* data, size_t size) {
  if (size < 16) return RAM_DISK_TYPE_UNKNOWN;
  if (size == 174848 || size == 175531) return RAM_DISK_TYPE_D64;
  if (size >= 8 && memcmp(data, "GCR-1541", 8) == 0) return RAM_DISK_TYPE_G64;
  if (size >= 13 && memcmp(data, "MNIB-1541-RAW", 13) == 0) return RAM_DISK_TYPE_NIB;
  return RAM_DISK_TYPE_UNKNOWN;
}

void ram_disk_init(void) {
  if (initialized) return;
  memset(slots, 0, sizeof(slots));
  for (int i = 0; i < RAM_DISK_MAX_SLOTS; i++) {
    slots[i].data = (uint8_t*)heap_caps_malloc(RAM_DISK_MAX_SIZE, MALLOC_CAP_SPIRAM);
    if (slots[i].data) {
      slots[i].loaded = false;
      slots[i].size = 0;
      slots[i].type = RAM_DISK_TYPE_NONE;
    }
  }
  initialized = true;
}

void ram_disk_clear_slot(int slot) {
  if (slot < 0 || slot >= RAM_DISK_MAX_SLOTS) return;
  slots[slot].loaded = false;
  slots[slot].size = 0;
  slots[slot].type = RAM_DISK_TYPE_NONE;
  slots[slot].filename[0] = '\0';
}

bool ram_disk_load(int slot, const uint8_t* data, size_t size, const char* filename) {
  if (slot < 0 || slot >= RAM_DISK_MAX_SLOTS || !data || size > RAM_DISK_MAX_SIZE) return false;
  memcpy(slots[slot].data, data, size);
  slots[slot].size = size;
  slots[slot].type = detect_type(data, size);
  slots[slot].loaded = true;
  strncpy(slots[slot].filename, filename ? filename : "upload.d64", sizeof(slots[slot].filename) - 1);
  slots[slot].filename[sizeof(slots[slot].filename) - 1] = '\0';
  return true;
}

ram_disk_slot_t* ram_disk_get_slot(int slot) {
  if (slot < 0 || slot >= RAM_DISK_MAX_SLOTS) return nullptr;
  return slots[slot].loaded ? &slots[slot] : nullptr;
}

int ram_disk_find_free_slot(void) {
  for (int i = 0; i < RAM_DISK_MAX_SLOTS; i++) {
    if (!slots[i].loaded) return i;
  }
  return -1;
}

size_t ram_disk_free_bytes(void) {
  return ESP.getFreePsram();
}
