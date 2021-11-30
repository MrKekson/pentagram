#include <Arduino.h>
#include "debug.h"

int DLED::_DBGLED[4] = {18, 5, 4, 2};

DLED::DLED()
{
    Serial.begin(9600);
    for (size_t i = 0; i < sizeof(DLED::_DBGLED); i++)
    {
        /* code */
        pinMode(DLED::_DBGLED[i], OUTPUT);
        Blink(i, 50);
    }
}

void DLED::Blink(int l, int d)
{
    if (l <= sizeof(_DBGLED))
    {
        digitalWrite(DLED::_DBGLED[l], HIGH);
        delay(d);
        digitalWrite(DLED::_DBGLED[l], LOW);
    }
}
