/**
 * ESP1541 Arduino - Web-based Commodore 1541 emulator
 *
 * Load D64/G64 disk images via WiFi from a browser.
 * Images stored in PSRAM, served to C64 via IEC bus (when HAL is ready).
 */

#include "web_disk.h"
#include "ram_disk.h"

void setup() {
  Serial.begin(921600);
  delay(1000);
  Serial.println("\n\nESP1541 Arduino - Web disk loader");

  if (!psramFound()) {
    Serial.println("ERROR: PSRAM not found! Need ESP32-WROVER.");
    while (1) delay(1000);
  }
  Serial.printf("PSRAM: %u bytes\n", ESP.getPsramSize());

  ram_disk_init();
  web_disk_setup();

  Serial.println("Connect to WiFi AP 'ESP1541', open http://192.168.4.1");
}

void loop() {
  web_disk_loop();
}
