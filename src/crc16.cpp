/**
  Author: sascha_lammers@gmx.de
*/

#include "crc16.h"

#if __AVR__

uint16_t crc16_update(uint16_t crc, uint8_t a)
{
    return _crc16_update(crc, a);
}

#else

uint16_t crc16_update(uint16_t crc, uint8_t data)
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

#endif

uint16_t _crc16_update(uint16_t crc, const uint8_t *data, size_t len)
{
    while(len--) {
        crc = crc16_update(crc, *data++);
    }
    return crc;
}
