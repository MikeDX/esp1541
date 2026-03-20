/**
 * ESP1541 - Minimal main for Pi1541 on ESP32
 * EXPERIMENTALZERO-style: WiFi web loader, ram_disk, IEC bus emulation
 */
#ifdef __ESP32__

#include <Arduino.h>
#include "web_disk.h"
#include "ram_disk.h"
#include "ram_rom.h"
#include "1541/ROMs.h"
#include "1541/options.h"
#include "1541/DiskCaddy.h"
#include "1541/Pi1541.h"
#include "1541/Screen.h"
#include "1541/ScreenLCD.h"
#include "1541/FileBrowser.h"
#include "1541/InputMappings.h"
#include "1541/iec_commands.h"
#include "1541/iec_bus.h"
#include "1541/DiskImage.h"
#include <cstring>
#include "rpi-mailbox-interface.h"
#ifndef BIT
#define BIT(n) (1UL << (n))  /* Restore after m6502.h undefs it for ESP32 */
#endif

#ifdef __ESP32__
uint32_t RPi_CpuId = 0;
#endif

extern void emulator(void);
extern ROMs roms;
extern Options options;
extern DiskCaddy diskCaddy;
extern Pi1541 pi1541;
extern Screen screen;
extern ScreenLCD* screenLCD;
extern uint8_t deviceID;
extern IEC_Commands m_IEC_Commands;
extern InputMappings* inputMappings;
#ifdef __ESP32__
extern uint8_t* s_u8Memory;
#endif

void esp1541_init(void) {
  Serial.begin(921600);
  delay(500);
  Serial.println("\n\nESP1541 - Pi1541 + IEC bus");

  if (!psramFound()) {
    Serial.println("ERROR: PSRAM required");
    while (1) delay(1000);
  }
  Serial.printf("PSRAM: %u bytes\n", (unsigned)ESP.getPsramSize());

  /* Allocate 1541 emulated RAM (48KB) from PSRAM to save internal DRAM */
  s_u8Memory = (uint8_t*)ps_malloc(0xc000);
  if (!s_u8Memory) {
    Serial.println("ERROR: Cannot allocate 1541 RAM");
    while (1) delay(1000);
  }
  memset(s_u8Memory, 0, 0xc000);

  DiskImage::InitESP32Buffers();

  ram_disk_init();
  ram_rom_init();
  web_disk_setup();

  // Default options (no options.txt on ESP32)
  static char opts[] = "disableSD2IECCommands=1\nonResetChangeToStartingFolder=1\n";
  options.Process(opts);

  // Load ROM from ram_rom if uploaded via web
  if (ram_rom_loaded()) {
    memcpy(roms.ROMImages[0], ram_rom_get(), ROMs::ROM_SIZE);
    strncpy(roms.ROMNames[0], "web", 255);
    roms.ROMValid[0] = true;
    Serial.println("ROM loaded from web");
  } else {
    Serial.println("Upload 1541 ROM (16KB) via web - no ROM yet");
    roms.ROMValid[0] = false;
  }

  // Init IEC GPIO (via esp32_gpio.h)
  IEC_Bus::SetSplitIECLines(options.SplitIECLines());
  IEC_Bus::SetInvertIECInputs(options.InvertIECInputs());
  IEC_Bus::SetInvertIECOutputs(options.InvertIECOutputs());
  IEC_Bus::SetIgnoreReset(options.IgnoreReset());
  IEC_Bus::SetRotaryEncoderEnable(false);

  pi1541.drive.SetVIA(&pi1541.VIA[1]);
  pi1541.VIA[0].GetPortB()->SetPortOut(0, IEC_Bus::PortB_OnPortOut);
  IEC_Bus::Initialise();

  screenLCD = 0;  // No LCD on minimal build
  diskCaddy.SetScreen(nullptr, screenLCD, &roms);
  inputMappings = new InputMappings();
  inputMappings->INPUT_BUTTON_ENTER = options.GetButtonEnter();
  inputMappings->INPUT_BUTTON_UP = options.GetButtonUp();
  inputMappings->INPUT_BUTTON_DOWN = options.GetButtonDown();
  inputMappings->INPUT_BUTTON_BACK = options.GetButtonBack();
  inputMappings->INPUT_BUTTON_INSERT = options.GetButtonInsert();

  // Load first ram_disk slot into DiskCaddy
  ram_disk_slot_t* slot = ram_disk_get_slot(0);
  if (slot && slot->loaded && slot->data && slot->size > 0) {
    if (diskCaddy.InsertFromBuffer(slot->filename, slot->data, slot->size, false)) {
      Serial.printf("Mounted: %s\n", slot->filename);
    }
  }

  Serial.println("Starting emulator - connect C64 to IEC pins 12,13,14,15");
}

void esp1541_run(void) {
  emulator();  // Never returns
}

#endif /* __ESP32__ */
