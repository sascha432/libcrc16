/**
 * Author: sascha_lammers@gmx.de
 */

#include <Arduino.h>
#include "crc16.h"

char buf[128];

void setup()
{
    #define TEXT "The quick brown fox jumps over the lazy dog!"
    #define TEXT_CRC 0x7168
    constexpr uint16_t kTextCrc = TEXT_CRC;
    constexpr uint16_t uint16Array[] = { 26708/*Th*/, 8293/*e */, 30065/*qu*/, 25449/*ic*/, 8299/*k */, 29282/*br*/, 30575/*ow*/, 8302/*n */, 28518/*fo*/, 8312/*x */, 30058/*ju*/, 28781/*mp*/, 8307/*s */, 30319/*ov*/, 29285/*er*/, 29728/* t*/, 25960/*he*/, 27680/* l*/, 31329/*az*/, 8313/*y */, 28516/*do*/, 8551/*g!*/  };
    constexpr uint32_t intArray[4] = {10, 200, 3000, 40000 };
    auto text_P = PSTR(TEXT);
    char text[strlen_P(text_P) + 1];
    strcpy_P(text, text_P);

    constexpr uint16_t crc4 = constexpr_crc16_update(TEXT, constexpr_crc16_strlen(TEXT));
    static_assert(crc4 == kTextCrc, "something went wrong");
    constexpr uint16_t crc6 = constexpr_crc16_update(uint16Array, sizeof(uint16Array));
    constexpr uint16_t crc7 = constexpr_crc16_update(intArray, sizeof(intArray));

    Serial.begin(115200);

    Serial.print(text);

    uint16_t crc2 = crc16_update(text, strlen(text));
    if (crc2 != kTextCrc) {
        snprintf_P(buf, sizeof(buf), PSTR("WARNING!!! Something went wrong, the CRC is %04x instead of %04x\n"), crc2, TEXT_CRC);
        Serial.print(buf);
        delay(10000);
    }

    uint16_t crc3 = ~0;
    auto ptr = text;
    while(*ptr) {
        crc3 = crc16_update(crc3, *ptr++);
    }

    uint16_t crc = ~0;
    ptr = strtok(text, " ");
    while(ptr) {
        crc = crc16_update(crc, ptr, strlen(ptr));
        ptr = strtok(nullptr, " ");
        if (!ptr) {
            break;
        }
        crc = crc16_update(crc, ' ');
    }

    uint16_t crc5 = crc16_update_P(text_P, strlen_P(text_P));

    snprintf_P(buf, sizeof(buf), PSTR(": %04x %04x %04x %04x %04x %04x %04x\n"), crc2, crc3, crc, crc4, crc5, crc6, crc7);
    Serial.println(buf);
}

void loop()
{
    delay(1);
}