/**
  Author: sascha_lammers@gmx.de
*/

#include "crc16.h"

#if !CRC16_UPDATE_INLINED

uint16_t __crc16_update(uint16_t crc, uint8_t data)
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
        crc = _crc16_update(crc, *data++);
    }
    return crc;
}

uint16_t _crc16_update_P(uint16_t crc, const uint8_t *data, size_t len)
{
    while(len--) {
        crc = _crc16_update(crc, pgm_read_byte(data));
        data++;
    }
    return crc;
}
