#pragma once

#include <FastLED.h>

#include "base.h"
#include "effects.h"

struct CData
{
    CData()
    {
        rWeight = 0;
        c = CHSV(0, 0, 0);
    }
    CData(int rw, CHSV col)
    {
        rWeight = rw;
        c = CHSV(col);
    }

    int rWeight = 0; /*render weight*/
    CHSV c;          /* _data */
};

struct WCData
{
    WCData()
    {
        currentWeight = 0;
        h = 0;
        s = 0;
        v = 0;
    }

    WCData(int cw, int rw, int _h, int _s, int _v)
    {
        currentWeight = cw;
        h = _h;
        s = _s;
        v = _v;
    }

    int currentWeight = 0; /*render weight*/
    int h = 0;
    int s = 0;
    int v = 0; /* _data */
};

class Renderer
{
private:
    int brightness = DEFAULT_BRIGHTNESS;

    CHSV _data[NUM_DEG];
    CRGB _leds[NUM_LEDS];

public:
    Renderer();
    ~Renderer();
    void Render(const std::vector<BaseEffect *> &effects);
    void Setup();
};

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Setup()
{
    FastLED.addLeds<NEOPIXEL, OUTPUT_PIN>(_leds, NUM_LEDS);
    FastLED.setBrightness(brightness);
    FastLED.clear();

    fill_solid(_leds, NUM_LEDS, CRGB(96, 0, 0));
    FastLED.show();
    delay(200);
}

void Renderer::Render(const std::vector<BaseEffect *> &effects)
{
    int led_width = 3;

    WCData _weightedData[NUM_DEG];

    for (BaseEffect *e : effects)
    {
        if (e->isRunning)
        {
            double halfWidth = e->_width / 2.0;
            double minDeg = e->_deg - halfWidth;
            double maxDeg = e->_deg + halfWidth;

            // Serial.print('\n');
            // Serial.print(halfWidth);
            // Serial.print(" -- ");
            // Serial.print(e->_deg);
            // Serial.print(" -- ");
            // Serial.print(minDeg);
            // Serial.print(" -- ");
            // Serial.print(maxDeg);
            // Serial.print(" \n ");

            for (int i = minDeg; i <= maxDeg; i++)
            {
                int j = Clamp(i);

                _weightedData[j].h += e->_c.h * e->_rWeight;
                _weightedData[j].s += e->_c.s * e->_rWeight;
                _weightedData[j].v += e->_c.v * e->_rWeight;
                _weightedData[j].currentWeight += e->_rWeight;

                // Serial.print(' ');
                // Serial.print(_weightedData[j].currentWeight);
            }
        }

        // Serial.print('\n');
    }

    // Serial.print("\n rendering 360 \n");
    for (int i = 0; i < NUM_DEG; ++i)
    {
        // Serial.print("i:");
        // Serial.print(i);
        // Serial.print(" h:");
        // Serial.print(_weightedData[i].h);
        // Serial.print(" w: ");
        // Serial.print(_weightedData[i].currentWeight);
        // Serial.print(" \n: ");

        if (_weightedData[i].currentWeight > 0)
        {
            _data[i].h = _weightedData[i].h / _weightedData[i].currentWeight;
            _data[i].s = _weightedData[i].s / _weightedData[i].currentWeight;
            _data[i].v = _weightedData[i].v / _weightedData[i].currentWeight;
        }
        else
        {
            _data[i] = CHSV(0, 0, 0);
        }
    }
    // Serial.print(" \n: ");

    // Serial.print("rendering leds \n");

    for (int i = 0; i < NUM_LEDS; i++)
    {
        int di = i * LED_RATIO - POS_CORRECTION;

        int h = 0, s = 0, v = 0, count = 0;
        int lnum = 0;

        for (int dnum = di - led_width; dnum < di + led_width; dnum++)
        {
            count++;
            lnum = Clamp(dnum);

            h += _data[lnum].h;
            s += _data[lnum].s;
            v += _data[lnum].v;

            if (dnum == di)
            {
                count += 2;
                lnum = Clamp(dnum);

                h += _data[lnum].h * 2;
                s += _data[lnum].s * 2;
                v += _data[lnum].v * 2;
            }

            // weighted in middle, but maybe not ok
        }
        h = h / count;
        s = s / count;
        v = v / count;

        _leds[NUM_LEDS - 1 - i] = CHSV(h, s, v);
    }

    FastLED.show();
}