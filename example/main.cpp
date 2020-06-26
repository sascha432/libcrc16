/**
 * Author: sascha_lammers@gmx.de
 */

#include <Arduino.h>
#include "crc16.h"

char buf[32];

void setup()
{
    auto text1 = PSTR("The quick brown fox jumps over the lazy dog");
    char text[strlen_P(text1) + 1];

    constexpr uint16_t crc4 = constexpr_crc16_update("The quick brown fox jumps over the lazy dog", 43);

    Serial.begin(115200);

    strcpy_P(text, text1);
    Serial.print(text);

    uint16_t crc2 = crc16_update(text, strlen(text));

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

    snprintf_P(buf, sizeof(buf), PSTR(": %04x %04x %04x %04x\n"), crc2, crc3, crc, crc4);
    Serial.println(buf);
}

void loop()
{
    delay(1);
}