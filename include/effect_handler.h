#pragma once

#include <FastLED.h>
#include <vector.h>

#include "effect.h"
#include "debug.h"
#include "base.h"

#define MAX_EFFECTS 16

int brightness = DEFAULT_BRIGHTNESS;
// circle map to leds, mixing and displaying running effect arrays
class EffectHandler
{
private:
    std::vector<Effect *> effects;
    CRGB leds[NUM_LEDS];
    CHSV _data[NUM_DEG];
    int _last_time;
    double r_weight = 1;

    int Clamp(int degNum, int clamp = 360)
    {
        int current = degNum;
        if (degNum < 0)
        {
            current = clamp + degNum; // = CData(ww, c);
        }

        if (degNum >= 360)
        {
            current = degNum - clamp; // = CData(ww, c);
        }
        return current;
    }

    /*CHSV Valami(int degNum, double width = 5, int clamp = 360)
    {
        int h = 0, s = 0, v = 0, count = 0;
        int current = 0;
        for (int i = degNum - width / 2; i < degNum + width / 2; i++)
        {
            count++;
            current = i;
            if (i < 0)
            {
                current = clamp + i; // = CData(ww, c);
            }

            if (i >= 360)
            {
                current = i - clamp; // = CData(ww, c);
            }

            h += _data[current].h;
        }

        h = h / count;
        v = v / count;
        s = s / count;

        CHSV retCol = CHSV(h, s, v);
        return retCol;
    }*/

public:
    void
    AddEffect(Effect *e)
    {
        effects.push_back(e);
    }

    void RemoveEffect(Effect *e)
    {
        effects.erase(std::remove(effects.begin(), effects.end(), e), effects.end());
    }

    ~EffectHandler()
    {
        for (auto e : effects)
        {
            delete e;
        }
    }

    void Render()
    {

        int led_width = 3;
        // int led_width = LED_RATIO;
        //  update effects
        for (auto e : effects)
        {
            e->DoStuff();
        }
        DLED::Blink(0);

        // mix effect outputs
        if (effects.size() > 0)
        {
            for (int i = 0; i < NUM_DEG; ++i)
            {
                int rcount = 0;
                int h, s, v;
                int cWeight = effects[0]->_data[i].rWeight;
                DLED::Blink(2, 0);
                if (cWeight > 0) // TODO: 0 +val for start?
                {

                    h = effects[0]->_data[i].c.h * cWeight;
                    s = effects[0]->_data[i].c.s * cWeight;
                    v = effects[0]->_data[i].c.v * cWeight;
                    rcount = cWeight;
                }
                for (size_t j = 1; j < effects.size(); j++)
                {
                    cWeight = effects[j]->_data[i].rWeight;
                    if (cWeight > 0)
                    {
                        // Blink(OP2, 0);
                        rcount += cWeight;
                        h += effects[j]->_data[i].c.h * cWeight;
                        v += effects[j]->_data[i].c.v * cWeight;
                        s += effects[j]->_data[i].c.s * cWeight;
                    }
                }

                // if (1 > 0)
                // {

                // DEBUG REMOVE!
                _data[i] = CHSV(0, 128, 128);

                if (rcount > 0)
                {

                    h = h / rcount;
                    s = s / rcount;
                    v = v / rcount;

                    _data[i] = CHSV(h, s, v);
                }
            }

            DLED::Blink(1, 0);
            for (int i = 0; i < NUM_LEDS; ++i)
            {
                int di = i * LED_RATIO;

                int h = 0, s = 0, v = 0, count = 0;
                int lnum = 0;

                // leds[i] = _data[di]; //<- ez tré, csúszó átlag?
                for (int dnum = di - led_width; dnum < di + led_width; dnum++)
                {
                    count++;
                    lnum = Clamp(dnum);

                    h += _data[lnum].h;
                    s += _data[lnum].s;
                    v += _data[lnum].v;

                    // should be mid weighted
                }
                h = h / count;
                s = s / count;
                v = v / count;

                leds[i] = CHSV(h, s, v);
            }
        }
        else
        {
            // Error, random colour in every 2sec
            auto c = CRGB(rand() % 128, rand() % 128, rand() % 128);
            for (int i = 0; i < NUM_LEDS; ++i)
            {
                leds[i] = c;
            }
            delay(2000);
        }
        FastLED.show();
    }

    void Start()
    {
        FastLED.addLeds<NEOPIXEL, OUTPUT_PIN>(leds, NUM_LEDS);
        FastLED.setBrightness(brightness);
        FastLED.clear();

        fill_solid(leds, NUM_LEDS, CRGB(192, 32, 32));
        FastLED.show();
        delay(200);

        this->AddEffect(new Effect(CHSV(160, 150, 128), 1));
        //  //this->AddEffect(new Wobble(CRGB::AliceBlue, 2));
        //  //
        this->AddEffect(new SymbolSimpleFade(CHSV(0, 192, 192), 6, 10000, 200));
        // this->AddEffect(new DeepLightEffect(CHSV(180, 200, 192), 8, 8, -4, 256));
        this->AddEffect(new LightSection(70, 10, .5, false, CHSV(70, 192, 192), 130));
        // this->AddEffect(new LightSection(282, 45, 3.14, true, CHSV(180, 255, 198), 130));
        this->AddEffect(new LightSection(40, 15, 1, false, CHSV(140, 200, 180), 130));

        _last_time = millis();
    }
};
