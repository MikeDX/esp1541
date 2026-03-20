# ESP1541

Port of **[Pi1541](https://github.com/pi1541/Pi1541)** (Commodore 1541 drive emulation) toward **ESP32**, with a **WiFi web UI** to load D64/G64 images into PSRAM—no SD card required for disk images.

This repo is **not** FujiNet; it is focused on the C64 **IEC serial bus** and 1541 behaviour.

---

## Current status (what works today)

| Area | Status |
|------|--------|
| **Web disk loader** | **Works.** ESP32 creates AP `ESP1541` (password `15411541`). Browser at `http://192.168.4.1` uploads D64/G64 into PSRAM (4 slots × 512 KB). |
| **PlatformIO builds** | **Works** for `esp1541` (classic ESP32-WROVER) and **`esp1541-s3`** (ESP32-S3 + PSRAM). |
| **Pi1541 core (`src/1541/`)** | **Present but not linked** in default builds. The upstream codebase is large; linking it on classic ESP32 overflowed internal DRAM (~176 KB over). |
| **IEC bus → C64** | **HAL started** (`src/esp32_hal/`, GPIO mapping in `rpi_gpio_stub.h`). Full emulation loop is wired in `main_esp1541.cpp` + `kernel_main` paths but **disabled** until the firmware fits RAM cleanly. |
| **1541 ROM** | Intended flow: upload **16 KB 1541 ROM** via web (`ram_rom`); see `main_esp1541.cpp`. You must supply your own legally obtained ROM. |

**Bottom line:** you can **flash and use the web loader** today. **Drive emulation on the IEC bus** is the active integration goal; **ESP32-S3 with PSRAM** is the recommended platform for the next step (more internal RAM + faster PSRAM than original ESP32).

---

## Build (PlatformIO)

```bash
# Default: classic ESP32-WROVER-KIT (web loader firmware)
pio run
pio run -t upload

# ESP32-S3 DevKitC-1 (or compatible module with OPI PSRAM, e.g. N8R2 / N16R8)
pio run -e esp1541-s3
pio run -e esp1541-s3 -t upload
```

Shared settings live under the unnamed `[env]` section in `platformio.ini` (libraries, `build_src_filter`, LittleFS). Per-board overrides are only `board`, `build_flags`, and (for S3) `board_build.arduino.memory_type`.

---

## Hardware

### Web loader only (any env)

- **ESP32-WROVER** or **ESP32-S3** with **PSRAM** (strongly recommended for multiple disk slots).
- USB for power + serial monitor.

### IEC bus to C64 (when emulation is enabled)

- **ATN, DATA, CLOCK** (and **RESET** if you implement/use it) must be **level-shifted** between **3.3 V (ESP32)** and **5 V (C64)** (e.g. 74HCT245 / dedicated IEC buffers).
- **Planned GPIO mapping** (see `src/esp32_hal/rpi_gpio_stub.h`):

  | Signal | ESP32 GPIO |
  |--------|------------|
  | ATN    | 12 |
  | CLOCK  | 13 |
  | DATA   | 14 |
  | RESET  | 15 |

Change pins in one place there if your PCB differs.

---

## Enabling full Pi1541 in the build (developers)

1. In `platformio.ini`, adjust `build_src_filter` for the env you use: **remove** `-<1541/>` and `-<main_esp1541.cpp>`.
2. Re-enable **`__ESP32__`** / **`EXPERIMENTALZERO`** and include paths if you had removed them (see git history / `README_ESP1541.md`).
3. Prefer **`esp1541-s3`**. If the linker still complains about `.dram0.bss`, enable placing BSS in PSRAM (`CONFIG_SPIRAM_ALLOW_BSS_SEG_EXTERNAL_MEMORY`) via `custom_sdkconfig` or `sdkconfig.defaults`, or move more globals to `heap_caps_malloc` / `EXT_RAM_BSS_ATTR` as already done for some buffers.

Details and memory notes: **`README_ESP1541.md`**.

---

## Layout

| Path | Role |
|------|------|
| `src/esp1541.ino` | Arduino entry: web loader |
| `src/web_disk.*`, `src/ram_disk.*`, `src/ram_rom.*` | WiFi AP, HTTP upload, PSRAM slots, optional ROM upload |
| `src/1541/` | Pi1541 sources (6502, VIA, drive, GCR, IEC logic) |
| `src/esp32_hal/` | ESP32 GPIO / timer shims for Pi1541’s `read32`/`write32` style |
| `include/` | Stubs (`ff.h`, `debug.h`, `diskio.h`, …) for ESP32 builds |

---

## License

Pi1541 is GPL v3; respect upstream and ROM copyright. ESP1541-specific files follow the same spirit unless noted otherwise.
