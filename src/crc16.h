/**
  Author: sascha_lammers@gmx.de
*/

#pragma once

#include <Arduino.h>

#if __AVR__

#include <util/crc16.h>

uint16_t crc16_update(uint16_t crc, uint8_t a);

#else

uint16_t crc16_update(uint16_t crc, uint8_t a);

static inline uint16_t _crc16_update(uint16_t crc, uint8_t a)
{
    return crc16_update(crc, a);
}

#endif

uint16_t _crc16_update(uint16_t crc, const uint8_t *data, size_t len);

inline uint16_t crc16_update(uint16_t crc, const void *data, size_t len)
{
    return _crc16_update(crc, reinterpret_cast<const uint8_t *>(data), len);
}

inline uint16_t crc16_update(const void *data, size_t len)
{
    return crc16_update(~0, data, len);
}


constexpr uint16_t constexpr_crc16_update(uint16_t crc, int i)
{
    return i < 8 ? constexpr_crc16_update((crc & 1) ? ((crc >> 1) ^ 0xa001) : (crc >> 1), i + 1) : crc;
}

template<typename T>
constexpr uint16_t constexpr_crc16_update(uint16_t crc, const T *data, size_t len)
{
    return len == 0 ? crc : constexpr_crc16_update(constexpr_crc16_update(crc ^ *data, 0), data + 1, len - 1);
}

template<typename T>
constexpr uint16_t constexpr_crc16_update(const T *data, size_t len)
{
    return constexpr_crc16_update(~0, data, len);
}
