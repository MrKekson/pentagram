#pragma once

#include <FastLED.h>
#include <vector.h>

#include "effect.h"
#include "debug.h"

#define OUTPUT_PIN 13
#define NUM_LEDS 96
#define NUM_DEG 360
#define MAX_EFFECTS 16
#define ONBOARD_LED 35

int brightness = 120;
//circle map to leds, mixing and displaying running effect arrays
class EffectHandler
{
private:
    std::vector<Effect *> effects;
    CRGB leds[NUM_LEDS];
    CHSV _data[NUM_DEG];
    int _last_time;
    double r_weight = 1;

public:
    void AddEffect(Effect *e)
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
        //update effects
        for (auto e : effects)
        {
            e->DoStuff();
        }
        DLED::Blink(0);

        //mix effect outputs
        if (effects.size() > 0)
        {
            for (int i = 0; i < NUM_DEG; ++i)
            {
                int rcount = 0;
                int h, s, v;
                int cWeight = effects[0]->_data[i].rWeight;
                DLED::Blink(2, 0);
                if (cWeight > 0)
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
                        //Blink(OP2, 0);
                        rcount += cWeight;
                        h += effects[j]->_data[i].c.h * cWeight;
                        v += effects[j]->_data[i].c.v * cWeight;
                        s += effects[j]->_data[i].c.s * cWeight;
                    }
                }

                // if (1 > 0)
                // {

                //DEBUG REMOVE!
                _data[i] = CHSV(0, 128, 128);

                if (rcount > 0)
                {

                    h = h / rcount;
                    s = s / rcount;
                    v = v / rcount;

                    _data[i] = CHSV(h, s, v);
                }
            }

            DLED::Blink(1, 1);
            for (int i = 0; i < NUM_LEDS; ++i)
            {
                int di = i * 3.75 - 1;

                leds[i] = _data[di]; //<- ez tré, csúszó átlag?
            }
        }
        else
        {
            //Error, random colour in every 2sec
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

        fill_solid(leds, NUM_LEDS, CRGB(128, 128, 128));
        FastLED.show();
        delay(100);

        this->AddEffect(new Effect(CHSV(160, 150, 128), 64));
        //this->AddEffect(new Wobble(CRGB::AliceBlue, 2));
        //

        this->AddEffect(new DeepLightEffect(CHSV(160, 255, 192), 256));
        this->AddEffect(new LightSection(70, 10, .5, false, CHSV(70, 192, 192), 80));
        this->AddEffect(new LightSection(282, 45, 3.14, true, CHSV(180, 255, 198), 130));
        this->AddEffect(new LightSection(40, 15, 1, false, CHSV(140, 200, 180), 160));

        _last_time = millis();
    }
};
