# CRC16 library

Using util/crc16.h for AVR and a C++ implementation for ESP8266/ESP32 etc...

## Changelog

### v1.0.1

- Added strlen function for constexpr
- Updated library.json
- Added PROGMEM support

## Algorithm

CRC_MODBUS

```
- Width         = 16
- Poly          = 0x8005
- XorIn         = 0xffff
- ReflectIn     = True
- XorOut        = 0x0000
- ReflectOut    = True
```

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

``` c++
uint16_t crc16_update(const void *data, size_t len);
uint16_t crc16_update(uint16_t crc, const void *data, size_t len);
uint16_t crc16_update(uint16_t crc, uint8_t data);
```

### Functions with PROGMEM support

``` c++
uint16_t crc16_update_P(const void *data, size_t len);
uint16_t crc16_update_P(uint16_t crc, const void *data, size_t len);
uint16_t crc16_update(const __FlashStringHelper *str);
uint16_t crc16_update(uint16_t crc, const __FlashStringHelper *str);
```

### Different implementations

Some platforms like AVR have a different implementation of the update function. Set `__AVR_USE_NATIVE_CRC16_UPDATE` to `0` to use the C implementation, instead of the native one.

``` c++
inline uint16_t _crc16_update(uint16_t crc, uint8_t data);
```

## Recursive constexpr

The constexpr functions only accept arrays of char, uint8_t, uint16_t, uint24_t, uint32_t and uint64_t (as well as all signed versions). It cannot calculate it for double, structures or any data blobs. Depending on the underlying memory structures that the compiler uses, big vs little endian, alignment and padding, only char and signed char arrays are certain to produce the correct results.

To disable constexpr functions set `_CRC16_ADD_CONSTEXPR_FUNCTIONS` to `0`

Tested with gcc version 4.8.2

``` c++
constexpr uint16_t constexpr_crc16_update(const TYPE *data, size_t len);
constexpr uint16_t constexpr_crc16_update(uint16_t crc, int i);
constexpr uint16_t constexpr_crc16_update(uint16_t crc, const TYPE *data, size_t len);
constexpr size_t constexpr_crc16_strlen(const char *data, size_t len = 0);
```

