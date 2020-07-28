# CRC16 library

Using util/crc16.h for AVR and a C++ implementation for ESP8266/ESP32 etc...

## Changelog

- Added PROGMEM support

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

### Functions with PROGMEM support

Define CRC16_PROGMEM_SUPPORT=0 to disable PROGMEM support.

```
uint16_t crc16_update_P(PGM_VOID_P data, size_t len);
uint16_t crc16_update_P(uint16_t crc, PGM_VOID_P data, size_t len);
uint16_t crc16_update(const __FlashStringHelper *str);
uint16_t crc16_update(uint16_t crc, const __FlashStringHelper *str);
```

### Different implementations

Some platforms like AVR have a different implementation of the update function. Define __AVR_USE_NATIVE_CRC16_UPDATE=0 to use the C implementation.


```
inline uint16_t _crc16_update(uint16_t crc, uint8_t data);
```

## Recursive constexpr

Tested with gcc version 4.8.2

```
constexpr uint16_t constexpr_crc16_update(uint16_t crc, int i);
constexpr uint16_t constexpr_crc16_update(uint16_t crc, const TYPE *data, size_t len);
constexpr uint16_t constexpr_crc16_update(const TYPE *data, size_t len);
```

### How to verify it is really a constexpr

Following code will not compile:

```
constexpr uint16_t test_me = constexpr_crc16_update("test", 4);
```
