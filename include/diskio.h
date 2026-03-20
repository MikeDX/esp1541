/**
 * Disk I/O stub for ESP32 - Pi1541 uses disk_setEMM, disk_setUSB
 */
#ifndef DISKIO_H
#define DISKIO_H

#ifdef __ESP32__
// Stub - no EMMC/USB on ESP32
static inline void disk_setEMM(void* p) { (void)p; }
static inline void disk_setUSB(int n) { (void)n; }
#else
// Raspberry Pi - from original diskio
extern void disk_setEMM(void* p);
extern void disk_setUSB(int n);
#endif

#endif /* DISKIO_H */
