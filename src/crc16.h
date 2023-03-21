/**
  Author: sascha_lammers@gmx.de
*/

// Model: crc-16-modbus

#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__AVR__)

#include <avr/pgmspace.h>

#ifndef __AVR_USE_NATIVE_CRC16_UPDATE
#define __AVR_USE_NATIVE_CRC16_UPDATE 1
#endif

#elif defined(ESP8266) || defined(ESP32)

#include <pgmspace.h>

#endif

#ifndef _CRC16_UPDATE_INLINED
#define _CRC16_UPDATE_INLINED 1
#endif

#ifndef __AVR_USE_NATIVE_CRC16_UPDATE
#define __AVR_USE_NATIVE_CRC16_UPDATE 0
#endif

#ifndef _CRC16_ADD_CONSTEXPR_FUNCTIONS
#define _CRC16_ADD_CONSTEXPR_FUNCTIONS 1
#endif

#if __AVR_USE_NATIVE_CRC16_UPDATE

#include <util/crc16.h>

#elif _CRC16_UPDATE_INLINED

inline uint16_t _crc16_update(uint16_t crc, uint8_t data)

{
    crc ^= data;
    for (uint8_t i = 0; i < 8; ++i)
    {
        if (crc & 1)
        {
            crc = (crc >> 1) ^ 0xA001; // this is 0x8005 in reverse
        }
        else
        {
            crc = (crc >> 1);
        }
    }
    return crc;
}

#else

uint16_t __crc16_update(uint16_t crc, uint8_t data);

inline uint16_t _crc16_update(uint16_t crc, uint8_t a)
{
    return __crc16_update(crc, a);
}

#endif

inline uint16_t crc16_update(uint16_t crc, uint8_t a)
{
    return _crc16_update(crc, a);
}

inline uint16_t crc16_update(uint16_t crc, const uint8_t *data, size_t len)
{
    while(len--) {
        crc = _crc16_update(crc, *data++);
    }
    return crc;
}

inline uint16_t crc16_update(uint16_t crc, const void *data, size_t len)
{
    return crc16_update(crc, reinterpret_cast<const uint8_t *>(data), len);
}

inline uint16_t crc16_update(const void *data, size_t len)
{
    return crc16_update(~0, reinterpret_cast<const uint8_t *>(data), len);
}

#ifdef PROGMEM

inline uint16_t crc16_update_P(uint16_t crc, const uint8_t *data, size_t len)
{
    while(len--) {
        crc = _crc16_update(crc, pgm_read_byte(data++));
    }
    return crc;
}

inline uint16_t crc16_update_P(uint16_t crc, const void *data, size_t len)
{
    return crc16_update_P(crc, static_cast<const uint8_t *>(data), len);
}

inline uint16_t crc16_update_P(const void *data, size_t len)
{
    return crc16_update_P(~0, data, len);
}

struct __FlashStringHelper;

inline uint16_t crc16_update(uint16_t crc, const __FlashStringHelper *str)
{
    return crc16_update_P(crc, str, strlen_P(reinterpret_cast<const char *>(str)));
}

inline uint16_t crc16_update(const __FlashStringHelper *str)
{
    return crc16_update(~0, str);
}

#endif

#if _CRC16_ADD_CONSTEXPR_FUNCTIONS

constexpr uint16_t _constexpr_crc16_update(uint16_t crc, int i = 0)
{
    return i < 8 ? _constexpr_crc16_update((crc & 1) ? ((crc >> 1) ^ 0xa001) : (crc >> 1), i + 1) : crc;
}

constexpr uint16_t _constexpr_crc16_update_add(uint16_t crc, uint8_t data)
{
    return _constexpr_crc16_update(crc ^ data);
}

template <typename T>
constexpr uint16_t _constexpr_convert_data_to_bytes(uint16_t crc, T data, size_t left)
{
    return left ?  _constexpr_convert_data_to_bytes(_constexpr_crc16_update_add(crc, data & 0xff), data >> 8, left - 1) : crc;
}

template <typename T>
constexpr uint16_t constexpr_crc16_update(uint16_t crc, T data, size_t len)
{
    return len == 0 ? crc : constexpr_crc16_update(_constexpr_convert_data_to_bytes(crc, data[0], sizeof(data[0])), &data[1], len - sizeof(data[0]));
}

template <typename T>
constexpr uint16_t constexpr_crc16_update(T data, size_t len)
{
    return constexpr_crc16_update(~0, data, len);
}

constexpr size_t constexpr_crc16_strlen(const char *data, size_t len = 0)
{
    return *data ? constexpr_crc16_strlen(data + 1, len + 1) : len;
}

#endif