#pragma once

#define FPS 80
#define FPS_HACK (1000000 / FPS) * 2
#define POS_CORRECTION -9

#define NUM_DEG 360
#define NUM_LEDS 96
#define LED_RATIO NUM_DEG / NUM_LEDS

#define OUTPUT_PIN 27

#define DEFAULT_BRIGHTNESS 90

#define SYMBOL_LED_WIDTH 26
#define SYMBOL_WIDTH 30
#define SYMBOL_OFFSET 23

#define MILLIS_IN_MINUTE (60 * 1000)
#define MICROS_IN_MINUTE (MILLIS_IN_MINUTE * 1000)

#define SEC_TO_MICRO 1000000

#define MAX_ANIM_LENGHT (5 * 60 * SEC_TO_MICRO)

struct ParsedEffectData
{
    ParsedEffectData()
    {
        baseColor = CHSV(180, 180, 64);
        brightness = DEFAULT_BRIGHTNESS;
    }
    ParsedEffectData(int b, CHSV baseC)
    {
        baseColor = baseC;
        brightness = b;
    }
    CHSV baseColor;
    int brightness;
};

struct DCHSV
{
    double H;
    double S;
    double V;
    // double deltaH;
    // double deltaS;
    // double deltaV;

    DCHSV()
    {
        H = 0;
        S = 92;
        V = 92;
    }
    DCHSV(double h, double s, double v) : H(h), S(s), V(v) {}
    DCHSV(CHSV c) : H(c.h), S(c.s), V(c.v) {}

    // implicit conversion
    operator CHSV() const { return CHSV(H, S, V); }

    // explicit conversion
    // explicit operator int *() const { return nullptr; }
};

struct ColorDelta
{
    double H;
    double S;
    double V;
    double deltaH;
    double deltaS;
    double deltaV;
    int chance; // x/100000
    int64_t minTime, maxTime;

    ColorDelta(double h, double s, double v, double dH, double dS, double dV, int chn, int64_t miT, int64_t mxT) : H(h), S(s), V(v), deltaH(dH), deltaS(dS), deltaV(dV), chance(chn), minTime(miT), maxTime(mxT) {}
    ColorDelta(DCHSV c, double dH = 0, double dS = 0, double dV = 0, int chn = 0, int64_t miT = 0, int64_t mxT = 0) : H(c.H), S(c.S), V(c.V), deltaH(dH), deltaS(dS), deltaV(dV), chance(chn), minTime(miT), maxTime(mxT) {}
};

struct SData
{
    SData()
    {
        shape = 0;
        symbol = 0;
        weight = 0;
        c = DCHSV(0, 0, 0);
    }
    SData(DCHSV col, int _symbol, int _weight = 128, int _shape = 0)
    {
        shape = _shape;
        symbol = _symbol;
        weight = _weight;
        c = col;
    }

    int shape;
    int symbol;
    int weight;
    DCHSV c;
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

// double Clamp(double degNum, double clamp = NUM_DEG) // cyrcle clamp
//  {
//      if (degNum >= 0)
//      {
//          return fmod(degNum, NUM_DEG);
//      }

//     if (degNum < 0)
//     {
//         return NUM_DEG - fmod(fabs(degNum), NUM_DEG);
//     }
// }
