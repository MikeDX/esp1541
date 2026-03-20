# ESP1541 source selection - supports multiple targets via PLATFORMIO_ENV
#   fujinet:  FujiNet (Atari 8-bit), excludes 1541 emulator
#   esp1541:  Pi1541 Commodore 1541 emulator (IEC bus)

FILE(GLOB_RECURSE app_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Target from scripts/set_cmake_target.py (written by PlatformIO pre-script)
set(PIO_ENV "fujinet")
if(EXISTS ${CMAKE_SOURCE_DIR}/cmake_target.txt)
    file(READ ${CMAKE_SOURCE_DIR}/cmake_target.txt PIO_ENV)
    string(STRIP ${PIO_ENV} PIO_ENV)
endif()

if(PIO_ENV STREQUAL "fujinet")
    # FujiNet only - exclude Pi1541 emulator, Arduino code, and its main
    list(FILTER app_sources EXCLUDE REGEX ".*/1541/.*")
    list(FILTER app_sources EXCLUDE REGEX ".*/arduino/.*")
    list(FILTER app_sources EXCLUDE REGEX ".*/main_esp1541\\.cpp$")
elseif(PIO_ENV STREQUAL "esp1541" OR PIO_ENV STREQUAL "esp1541-4mb")
    # ESP1541 - Pi1541 emulator, exclude FujiNet main and Arduino code
    list(FILTER app_sources EXCLUDE REGEX ".*/main\\.cpp$")
    list(FILTER app_sources EXCLUDE REGEX ".*/arduino/.*")
    list(APPEND app_sources ${CMAKE_CURRENT_SOURCE_DIR}/main_esp1541.cpp)
endif()

idf_component_register(SRCS ${app_sources} REQUIRES esp_psram fatfs driver)
