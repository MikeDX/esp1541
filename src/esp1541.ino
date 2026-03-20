/**
 * ESP1541 - Web-based Commodore 1541 disk loader
 * Load D64/G64 via WiFi into PSRAM. Pi1541 + IEC bus coming next.
 */

#include "web_disk.h"
#include "ram_disk.h"

void setup() {
  Serial.begin(921600);
  delay(1000);
  Serial.println("\n\nESP1541 - Web disk loader");

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
