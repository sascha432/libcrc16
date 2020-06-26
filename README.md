# CRC16 library

Using util/crc16.h for AVR and a C implementation for ESP8266/ESP32 etc...

## Description

from **util/crc16.h**

```
Optimized CRC-16 calculation.

Polynomial: x^16 + x^15 + x^2 + 1 (0xa001)
Initial value: 0xffff

This CRC is normally used in disk-drive controllers.

The following is the equivalent functionality written in C.
```

## Functions

```
uint16_t crc16_update(uint16_t crc, uint8_t data);
uint16_t crc16_update(uint16_t crc, const void *data, size_t len);
uint16_t crc16_update(const void *data, size_t len);
```

```
// platform based implementation
inline uint16_t _crc16_update(uint16_t crc, uint8_t data);
```

## Recursive constexpr

```
constexpr uint16_t constexpr_crc16_update(uint16_t crc, int i);
constexpr uint16_t constexpr_crc16_update(uint16_t crc, const char/uint8_t *data, size_t len);
constexpr uint16_t constexpr_crc16_update(const char/uint8_t *data, size_t len);
```
