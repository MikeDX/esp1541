/**
 * Minimal FatFS type stubs for Pi1541 on ESP32
 * Types only - actual FatFS from ESP-IDF or separate lib for full build
 */
#ifndef FF_H
#define FF_H

#include <stdint.h>
#include "integer.h"

typedef enum {
    FR_OK = 0,
    FR_DISK_ERR,
    FR_INT_ERR,
    FR_NOT_READY,
    FR_NO_FILE,
    FR_NO_PATH,
    FR_INVALID_NAME,
    FR_DENIED,
    FR_EXIST,
    FR_INVALID_OBJECT,
    FR_WRITE_PROTECTED,
    FR_INVALID_DRIVE,
    FR_NOT_ENABLED,
    FR_NO_FILESYSTEM,
    FR_MKFS_ABORTED,
    FR_TIMEOUT,
    FR_LOCKED,
    FR_NOT_ENOUGH_CORE,
    FR_TOO_MANY_OPEN_FILES,
    FR_INVALID_PARAMETER
} FRESULT;

#define FA_READ         0x01
#define FA_WRITE        0x02
#define FA_OPEN_EXISTING 0x00
#define FA_CREATE_NEW   0x04
#define FA_CREATE_ALWAYS 0x08
#define FA_OPEN_ALWAYS  0x10

#define AM_RDO  0x01
#define AM_HID  0x02
#define AM_SYS  0x04
#define AM_DIR  0x10
#define AM_ARC  0x20

typedef struct {
    uint32_t fsize;
    uint16_t fdate;
    uint16_t ftime;
    uint8_t  fattrib;
    char     fname[64];
} FILINFO;

typedef struct {
    void* obj;
    uint8_t* buf;
    uint16_t fsize;
    uint16_t fptr;
    uint16_t clust;
    uint32_t sect;
} FIL;

typedef struct {
    void* obj;
    uint32_t dptr;
    uint32_t clust;
    uint32_t sect;
    uint8_t* dir;
    uint8_t* fn;
} DIR;

#ifdef __ESP32__
/* Stub implementations - ESP32 minimal build uses ram_disk */
#define FA_OPEN_APPEND 0
typedef struct { uint8_t csize; uint8_t dummy[63]; } FATFS;
typedef char TCHAR;
static inline void SetACTLed(bool) {}
static inline FRESULT f_open(FIL* fp, const char* path, uint8_t mode) {
    (void)fp; (void)path; (void)mode;
    return FR_NO_FILE;
}
static inline FRESULT f_read(FIL* fp, void* buf, uint32_t btr, uint32_t* br) {
    (void)fp; (void)buf; (void)btr; if (br) *br = 0;
    return FR_OK;
}
static inline FRESULT f_write(FIL* fp, const void* buf, uint32_t btw, uint32_t* bw) {
    (void)fp; (void)buf; (void)btw; if (bw) *bw = 0;
    return FR_OK;
}
static inline FRESULT f_close(FIL* fp) {
    (void)fp;
    return FR_OK;
}
static inline FRESULT f_lseek(FIL* fp, uint32_t ofs) {
    (void)fp; (void)ofs;
    return FR_OK;
}
static inline FRESULT f_opendir(DIR* dp, const char* path) {
    (void)dp; (void)path;
    return FR_NO_FILE;
}
static inline FRESULT f_readdir(DIR* dp, FILINFO* fno) {
    (void)dp; if (fno) fno->fname[0] = 0;
    return FR_NO_FILE;
}
static inline FRESULT f_closedir(DIR* dp) {
    (void)dp;
    return FR_OK;
}
static inline FRESULT f_stat(const char* path, FILINFO* fno) {
    (void)path; (void)fno;
    return FR_NO_FILE;
}
static inline FRESULT f_getcwd(char* buf, uint32_t len) {
    (void)buf; (void)len;
    return FR_NO_FILE;
}
static inline FRESULT f_findfirst(DIR* dp, FILINFO* fno, const char* path, const char* pattern) {
    (void)dp; (void)fno; (void)path; (void)pattern;
    return FR_NO_FILE;
}
static inline FRESULT f_findnext(DIR* dp, FILINFO* fno) {
    (void)dp; (void)fno;
    return FR_NO_FILE;
}
static inline FRESULT f_getfree(const char* path, uint32_t* nclst, FATFS** fatfs) {
    (void)path; if (nclst) *nclst = 0; if (fatfs) *fatfs = 0;
    return FR_NO_FILE;
}
static inline FRESULT f_mount(FATFS* fs, const char* path, uint8_t opt) {
    (void)fs; (void)path; (void)opt;
    return FR_OK;
}
static inline FRESULT f_chdrive(const char* path) {
    (void)path;
    return FR_OK;
}
static inline FRESULT f_chdir(const char* path) {
    (void)path;
    return FR_OK;
}
static inline FRESULT f_mkdir(const char* path) {
    (void)path;
    return FR_NO_FILE;
}
static inline FRESULT f_unlink(const char* path) {
    (void)path;
    return FR_NO_FILE;
}
static inline FRESULT f_rename(const char* oldname, const char* newname) {
    (void)oldname; (void)newname;
    return FR_NO_FILE;
}
static inline FRESULT f_chmod(const char* path, uint8_t attr, uint8_t mask) {
    (void)path; (void)attr; (void)mask;
    return FR_OK;
}
static inline FRESULT f_getlabel(const char* path, char* label, uint32_t* vsn) {
    (void)path; if (label) label[0]=0; if (vsn) *vsn=0;
    return FR_NO_FILE;
}
typedef uint32_t FSIZE_t;
static inline FSIZE_t f_size(FIL* fp) {
    (void)fp;
    return 0;
}
#endif

#endif /* FF_H */
