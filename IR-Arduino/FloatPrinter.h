//
// Created by arne on 30.08.17.
//

#ifndef IR_THERMO_FLOATPRINTER_H
#define IR_THERMO_FLOATPRINTER_H

#include <Arduino.h>

char * ftoa(float f, unsigned int digits = 2)
{
    int index = 0;
    static char s[16];                    // buffer to build string representation
    // handle sign
    if (f < 0.0)
    {
        s[index++] = '-';
        f = -f;
    }
    // handle infinite values
    if (isinf(f) != 0)
    {
        strcpy(&s[index], "INF");
        return s;
    }
    // handle Not a Number
    if (isnan(f) != 0)
    {
        strcpy(&s[index], "NaN");
        return s;
    }

    // max digits
    if (digits > 6) digits = 6;
    auto multiplier = static_cast<long>(pow(10, digits));     // fix int => long

    auto exponent = int(log10(f));
    float g = f / pow(10, exponent);
    if ((g < 1.0) && (g != 0.0))
    {
        g *= 10;
        exponent--;
    }

    auto whole = long(g);                     // single digit
    auto part = long((g-whole)*multiplier);   // # digits
    char format[16];
    sprintf(format, "%%ld.%%0%dld E%%+d", digits);
    sprintf(&s[index], format, whole, part, exponent);

    return s;
}

void Serial_printFloat(float f) {
    Serial.print(ftoa(f));
}

void Serial_printlnFloat(float f) {
    Serial.println(ftoa(f));
}

void Serial_printf(const char* string, float val){
    Serial.print(string);
    Serial_printlnFloat(val);
}

void Serial_printf(const __FlashStringHelper * string, float val){
    Serial.print(string);
    Serial_printlnFloat(val);
}

void Serial_printf(const char* string, size_t val){
    Serial.print(string);
    Serial.println(val);
}

void Serial_printf(const char* string, uint32_t val){
    Serial.print(string);
    Serial.println(val);
}

void Serial_printf(const __FlashStringHelper * string, uint32_t val){
    Serial.print(string);
    Serial.println(val);
}

void Serial_printf(const char* string, uint8_t val){
    Serial.print(string);
    Serial.println(val);
}

void Serial_printf(const char* string, int8_t val){
    Serial.print(string);
    Serial.println(val);
}

void Serial_printf(const char* string, int32_t val){
    Serial.print(string);
    Serial.println(val);
}

#endif //IR_THERMO_FLOATPRINTER_H
