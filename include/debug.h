/**
 * Debug output for Pi1541 - ESP32 uses Serial
 */
#ifndef DEBUG_H
#define DEBUG_H

#ifdef __ESP32__
#include <cstdio>
#define Debug_printf(...) printf(__VA_ARGS__)
#else
#define Debug_printf(...) /* empty or printf(__VA_ARGS__) */
#endif

#endif /* DEBUG_H */
