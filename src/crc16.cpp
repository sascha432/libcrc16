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
