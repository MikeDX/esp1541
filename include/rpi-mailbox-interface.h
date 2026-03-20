/* Stub for rpi-mailbox-interface.h on ESP32 */
#ifndef RPI_MAILBOX_INTERFACE_H
#define RPI_MAILBOX_INTERFACE_H
#include <stdint.h>
extern uint32_t RPi_CpuId;

#ifdef __ESP32__
struct rpi_mailbox_property_t {
    union { uint32_t buffer_32[32]; uint8_t buffer_8[128]; } data;
};
#define TAG_GET_PHYSICAL_SIZE 0
#define TAG_GET_VIRTUAL_SIZE 0
#define TAG_GET_DEPTH 0
#define TAG_ALLOCATE_BUFFER 0
#define TAG_SET_PHYSICAL_SIZE 0
#define TAG_SET_VIRTUAL_SIZE 0
#define TAG_SET_DEPTH 0
#define TAG_GET_PITCH 0
static inline void RPI_PropertyInit(void) {}
static inline void RPI_PropertyAddTag(uint32_t t, ...) { (void)t; }
static inline void RPI_PropertyProcess(void) {}
static inline rpi_mailbox_property_t* RPI_PropertyGet(uint32_t t) { (void)t; return 0; }
#endif
#endif
