Credits
=======

This is an incomplete list of sources and people behind **ESP1541**. If you
belong here and we missed you, it was unintentional—please open an issue or PR.

### Pi1541 (upstream 1541 emulation)

https://github.com/pi1541/Pi1541  

**Stephen White** and Pi1541 contributors. The core in `src/1541/` is derived
from Pi1541 (GPLv3). See file headers in that tree.

### ESP1541 (ESP32 / ESP32-S3 port)

Port work, WiFi disk loader, RAM disk/ROM layers, ESP32 HAL shims (`src/esp32_hal/`,
`src/web_disk.*`, `src/ram_disk.*`, `src/ram_rom.*`, `main_esp1541.cpp`, build
and integration). People who have landed work in this tree (see git history),
with GitHub links instead of private contact details:

- **[MikeDX](https://github.com/MikeDX)** — **[esp1541](https://github.com/MikeDX/esp1541)**
- **[idolpx](https://github.com/idolpx)** — upstream **[esp1541](https://github.com/idolpx/esp1541)**
- **James Johnston** — **[esp1541](https://github.com/MikeDX/esp1541)** (fork used for this line of development)

### Libraries (firmware)

- **ESPAsyncWebServer** / **AsyncTCP** — async HTTP for the web UI  
  https://github.com/me-no-dev/ESPAsyncWebServer (and ESP32Async AsyncTCP via
  PlatformIO dependencies).

- **Arduino-ESP32** / **Espressif** — board support and device SDKs used by
  PlatformIO (`espressif32` platform).

### Avery Lee, Matthias Reichl, Mark Simonson (not used here)

An older `CREDITS.md` in this repo named **Avery Lee** ([Altirra](http://www.virtualdub.org/altirra.html)),
**Matthias Reichl** ([PicoBoot / MyPicoDos](https://www.horus.com/~hias/atari/)), and **Mark Simonson**
([Atari 8-bit fonts](http://members.bitstream.net/marksim/atarimac/fonts.html)). Those projects are
**Atari-related** (docs, bootloader, fonts). **None of that code or data appears in the current
ESP1541 source tree**, so there is nothing to license or attribute from them *in this firmware*.
The links are kept only as pointers to their work and to clear up the earlier mix-up. If we ever
ship their materials, we will credit them properly in context.

### You

1541 **ROM** and **disk images** are not supplied; you must use media you are
legally entitled to use.
