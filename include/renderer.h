#pragma once

#include <FastLED.h>

#include "base.h"
#include "effects.h"

class Renderer
{
private:
    int brightness = DEFAULT_BRIGHTNESS;

    CHSV _data[NUM_DEG];
    CRGB _leds[NUM_LEDS];

public:
    Renderer();
    ~Renderer();
    void Render(const std::vector<BaseEffect *> &effectDatas);
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

void Renderer::Render(const std::vector<BaseEffect *> &effectDatas)
{
    int led_width = 3;
    // mix effect outputs by their weight
    if (effectDatas.size() > 0)
    {
        for (int i = 0; i < NUM_DEG; ++i)
        {
            int rcount = 0;
            int h, s, v;
            int cWeight = effectDatas[0]->_data[i].rWeight;

            h = 0;
            s = 0;
            v = 0;

            for (size_t j = 0; j < effectDatas.size(); j++)
            {
                cWeight = effectDatas[j]->_data[i].rWeight;
                if (cWeight > 0)
                {
                    rcount += cWeight;
                    h += effectDatas[j]->_data[i].c.h * cWeight;
                    v += effectDatas[j]->_data[i].c.v * cWeight;
                    s += effectDatas[j]->_data[i].c.s * cWeight;
                }
            }

            _data[i] = CHSV(0, 0, 0);

            if (rcount > 0)
            {

                h = h / rcount;
                s = s / rcount;
                v = v / rcount;

                _data[i] = CHSV(h, s, v);
            }
        }

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
    }
    else
    {
        // Error, random colour in every 2sec
        auto c = CRGB(255, 0, 0);
        for (int i = 0; i < NUM_LEDS; ++i)
        {
            _leds[i] = CRGB(255, 0, 0);
        }
    }
    FastLED.show();
}
