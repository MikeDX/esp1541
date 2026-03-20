/**
 * ESP1541 — Commodore 1541 on ESP32 (Pi1541 port)
 *
 * esp1541-s3: full IEC emulation + WiFi (upload ROM + D64/G64 first, then connects as drive)
 * esp1541:    web disk loader only (classic ESP32, not enough DRAM for full Pi1541 link)
 */

#include "ram_disk.h"

#if defined(USE_PI1541_EMU) && USE_PI1541_EMU
extern void esp1541_init(void);
extern void esp1541_run(void);

void setup() {
  esp1541_init();
  esp1541_run();
}

void loop() {}

#else

#include "web_disk.h"

void setup() {
  Serial.begin(921600);
  delay(1000);
  Serial.println("\n\nESP1541 - Web disk loader (use env:esp1541-s3 for full 1541 emulation)");
  if (!psramFound()) {
    Serial.println("ERROR: PSRAM not found!");
    while (1) delay(1000);
  }
  ram_disk_init();
  web_disk_setup();
  Serial.println("Connect to WiFi AP 'ESP1541', open http://192.168.4.1");
}

void loop() {
  web_disk_loop();
}

#endif
