# ESP1541 (PI1541 ported to ESP32)

Using [FujiNet](https://github.com/idolpx/fujinet-platformio) as a base.

* Requires an ESP32-WROVER-B that includes 4MB of PSRAM.

## Build Targets (PlatformIO)

Multiple targets via `pio run -e <target>`:

| Target | Description |
|--------|-------------|
| **fujinet** | FujiNet (Atari 8-bit networking). Excludes 1541 emulator. |
| **esp1541** | Pi1541 Commodore 1541 emulator for C64. IEC bus via GPIO + level shifters. |
| **esp1541-4mb** | Same as esp1541, for 4MB Flash boards. |

```bash
# FujiNet (default)
pio run -e fujinet

# Pi1541 1541 emulator (HAL in progress)
pio run -e esp1541
```

## ESP1541 Arduino (Web disk loader)

Standalone Arduino build with **web-based disk loading** – no SD card. Load D64/G64 images via WiFi into PSRAM.

```bash
cd arduino && pio run
```

- Creates WiFi AP "ESP1541" (password: 15411541)
- Open http://192.168.4.1 in a browser
- Upload D64/G64 files – they're stored in PSRAM (4 slots, 512KB each)
- Uses Arduino framework + ESPAsyncWebServer

Requires ESP32-WROVER (or similar with PSRAM). Pi1541 emulation + IEC bus integration coming next.

## ESP1541 Hardware

For the esp1541 target, connect to a C64 via the IEC serial bus:
- **ATN, DATA, CLOCK** - use level shifters (e.g. 74HCT245) for 3.3V ↔ 5V
- Pin assignments TBD in `src/1541/hal/`

Lots to do but a good start. Who wants to help?

