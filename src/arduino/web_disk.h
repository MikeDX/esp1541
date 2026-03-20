/**
 * Web server for disk image upload
 * Serves a simple page to upload D64/G64 files to PSRAM
 */

#ifndef WEB_DISK_H
#define WEB_DISK_H

void web_disk_setup(void);
void web_disk_loop(void);

#endif // WEB_DISK_H
