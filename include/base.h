#pragma once

#define POS_CORRECTION -9

#define NUM_DEG 360
#define NUM_LEDS 96
#define LED_RATIO NUM_DEG / NUM_LEDS

#define OUTPUT_PIN 27

#define DEFAULT_BRIGHTNESS 92

#define SYMBOL_LED_WIDTH 26
#define SYMBOL_WIDTH 30
#define SYMBOL_OFFSET 23

#define MILLIS_IN_MINUTE (60 * 1000)
#define MICROS_IN_MINUTE (MILLIS_IN_MINUTE * 1000)

#define SEC_TO_MICRO 1000000

#define MAX_ANIM_LENGHT (5 * 60 * SEC_TO_MICRO)

struct SData
{
    SData()
    {
        shape = 0;
        symbol = 0;
        weight = 0;
        c = CHSV(0, 0, 0);
    }
    SData(CHSV col, int _symbol, int _weight = 128, int _shape = 0)
    {
        shape = _shape;
        symbol = _symbol;
        weight = _weight;
        c = CHSV(col);
    }

    int shape;
    int symbol;
    int weight;
    CHSV c;
};

int Clamp(int degNum, int clamp = NUM_DEG) // cyrcle clamp
{
    if (degNum >= 0)
    {
        return degNum % NUM_DEG;
    }

    if (degNum < 0)
    {
        return NUM_DEG - (abs(degNum) % NUM_DEG);
    }
}

double Clamp(double degNum, double clamp = NUM_DEG) // cyrcle clamp
{
    if (degNum >= 0)
    {
        return fmod(degNum, NUM_DEG);
    }

    if (degNum < 0)
    {
        return NUM_DEG - fmod(fabs(degNum), NUM_DEG);
    }
}

double SymbolToDeg(int numba)
{
    if (numba < 0 || numba > 11)
    {
        return 0;
    }

    double pos = ((numba)*SYMBOL_WIDTH) + SYMBOL_OFFSET;
    return pos;
}

int DegToSymbol(double numba)
{
    double pos = int((numba - SYMBOL_OFFSET) / SYMBOL_WIDTH);
    return pos;
}
