# ESP1541 Arduino - Web Disk Loader

Load D64/G64 disk images via WiFi into PSRAM. No SD card needed.

## Build

```bash
cd arduino
pio run
```

## Upload

```bash
pio run -t upload
```

## Usage

1. Power on the ESP32
2. Connect to WiFi network **ESP1541** (password: 15411541)
3. Open http://192.168.4.1 in a browser
4. Select a D64 or G64 file and click "Load into drive"
5. Images are stored in PSRAM (4 slots, 512KB each)

## Hardware

- ESP32-WROVER or similar with PSRAM
- Board: esp-wrover-kit (change in platformio.ini for your hardware)

## Next steps

- Pi1541 emulation integration (read from RAM instead of SD)
- IEC bus GPIO for C64 connection
- Level shifters for 5V IEC bus
