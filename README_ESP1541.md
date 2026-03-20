# ESP1541 — Pi1541 integration notes

Supplement to **`README.md`** (project status and build targets).

## Memory (why S3 helps)

Pi1541 keeps large **static** structures (`ROMs`, `IEC_Commands`, per-`DiskImage` track buffers, etc.). On **classic ESP32**, linking the full tree exceeded **internal DRAM** even after moving some buffers to PSRAM with `heap_caps_malloc` / `EXT_RAM_ATTR`.

**ESP32-S3** with **OPI PSRAM** (`esp1541-s3` in `platformio.ini`) is the preferred target for turning **`src/1541/`** back on in `build_src_filter`.

`esp1541-s3` uses `board = esp32-s3-devkitc-1` with `board_build.arduino.memory_type = qio_opi` so **PSRAM is enabled** (use a module that actually has PSRAM, e.g. **N8R8**). If your PlatformIO reports an unknown board ID for another S3 board, keep this ID or pick any S3 board JSON that matches your flash/PSRAM and set `memory_type` accordingly.

## First boot / WiFi

1. Flash `esp1541-s3` and connect to the device AP (see serial log for SSID if needed).
2. Open the web UI: upload a **1541 ROM** (body must be **exactly 16384 bytes** — `POST /upload/rom`).
3. Upload a **D64/G64** (or compatible image) into slot 0.
4. The firmware waits until ROM + disk are present, then enters **1541 IEC emulation** (`EMULATING_1541`).

## Optional: BSS in external RAM

If the linker reports `dram0_0_seg` overflow after including `1541/`:

1. Try **`pio run -e esp1541-s3`** first.
2. Add to `platformio.ini` for that env:
   ```ini
   custom_sdkconfig = CONFIG_SPIRAM_ALLOW_BSS_SEG_EXTERNAL_MEMORY=y
   ```
   Then `pio run -t fullclean` and rebuild.
3. Ensure `sdkconfig` / Arduino-IDF merge actually applies the option (PlatformIO + Arduino can be picky; see Espressif docs).

## Runtime PSRAM allocations (already in tree)

Examples: `s_u8Memory` in `esp1541_init`, `DiskImage::InitESP32Buffers()`, `wd177x` sector buffer when tagged for external RAM.

## IEC GPIO

See **`src/esp32_hal/rpi_gpio_stub.h`** — single place to change pins for your PCB.
