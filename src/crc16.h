/**
  Author: sascha_lammers@gmx.de
*/

#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__AVR__)

#include <avr/pgmspace.h>

#ifndef CRC16_PROGMEM_SUPPORT
#define CRC16_PROGMEM_SUPPORT                   1
#endif

#ifndef __AVR_USE_NATIVE_CRC16_UPDATE
#define __AVR_USE_NATIVE_CRC16_UPDATE           1
#endif

#elif defined(ESP8266) || defined(ESP32)

#include <pgmspace.h>
#ifndef CRC16_PROGMEM_SUPPORT
#define CRC16_PROGMEM_SUPPORT                   1
#endif

#endif

#ifndef _CRC16_UPDATE_INLINED
#define _CRC16_UPDATE_INLINED                   1
#endif

#ifndef CRC16_PROGMEM_SUPPORT
#define CRC16_PROGMEM_SUPPORT                   0
#endif

#ifndef __AVR_USE_NATIVE_CRC16_UPDATE
#define __AVR_USE_NATIVE_CRC16_UPDATE           0
#endif

#if __AVR_USE_NATIVE_CRC16_UPDATE

#include <util/crc16.h>

#elif _CRC16_UPDATE_INLINED

static inline uint16_t _crc16_update(uint16_t crc, uint8_t data)
{
    crc ^= data;
    for (uint8_t i = 0; i < 8; ++i) {
        if (crc & 1) {
            crc = (crc >> 1) ^ 0xA001;
        } else {
            crc = (crc >> 1);
        }
    }
    return crc;
}

#else

uint16_t __crc16_update(uint16_t crc, uint8_t data);

static inline uint16_t _crc16_update(uint16_t crc, uint8_t a)
{
    return __crc16_update(crc, a);
}

#endif

uint16_t _crc16_update(uint16_t crc, const uint8_t *data, size_t len);

#if CRC16_PROGMEM_SUPPORT
uint16_t _crc16_update_P(uint16_t crc, const uint8_t *data, size_t len);
#endif

inline uint16_t crc16_update(uint16_t crc, uint8_t a)
{
    return _crc16_update(crc, a);
}

inline uint16_t crc16_update(uint16_t crc, const void *data, size_t len)
{
    return _crc16_update(crc, reinterpret_cast<const uint8_t *>(data), len);
}

inline uint16_t crc16_update(const void *data, size_t len)
{
    return crc16_update(~0, data, len);
}

#if CRC16_PROGMEM_SUPPORT

inline uint16_t crc16_update_P(uint16_t crc, PGM_VOID_P data, size_t len)
{
    return _crc16_update_P(crc, reinterpret_cast<const uint8_t *>(data), len);
}

inline uint16_t crc16_update_P(PGM_VOID_P data, size_t len)
{
    return crc16_update_P(~0, data, len);
}

inline uint16_t crc16_update(uint16_t crc, const __FlashStringHelper *str)
{
    return _crc16_update_P(crc, reinterpret_cast<const uint8_t *>(str), strlen_P(reinterpret_cast<PGM_P>(str)));
}

inline uint16_t crc16_update(const __FlashStringHelper *str)
{
    return crc16_update(~0, str);
}

#endif

constexpr uint16_t constexpr_crc16_update(uint16_t crc, int i)
{
    return i < 8 ? constexpr_crc16_update((crc & 1) ? ((crc >> 1) ^ 0xa001) : (crc >> 1), i + 1) : crc;
}

template<typename T>
constexpr uint16_t constexpr_crc16_update(uint16_t crc, const T *data, size_t len)
{
    return len == 0 ? crc : constexpr_crc16_update(constexpr_crc16_update(crc ^ (uint8_t)*data, 0), data + 1, len - 1);
}

template<typename T>
constexpr uint16_t constexpr_crc16_update(const T *data, size_t len)
{
    return constexpr_crc16_update(~0, data, len);
}
