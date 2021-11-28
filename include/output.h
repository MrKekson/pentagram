#include <FastLED.h>
#include <vector.h>

#define OUTPUT_PIN 13
#define NUM_LEDS 96
#define NUM_DEG 360
#define MAX_EFFECTS 16
#define ONBOARD_LED 35

#define OP0 18
#define OP1 5
#define OP2 4
#define OP3 2

int brightness = 20;
//circle map to leds, mixing and displaying running effect arrays

void Blink(int l, int d = 5)
{
    digitalWrite(l, HIGH);
    delay(d);
    digitalWrite(l, LOW);
}

struct CData
{
    CData()
    {
        doRender = false;
        c = CRGB(0, 0, 0);
    }
    CData(bool dorender, CRGB col)
    {
        doRender = dorender;
        c = CRGB(col);
    }
    bool doRender = false;
    CRGB c; /* data */
};

class Effect
{
protected:
    int _last_time;

public:
    CData data[NUM_DEG];

    virtual void DoStuff()
    {
        _last_time = millis();
    }

    explicit Effect(CRGB c, bool dsp = true)
    {
        _last_time = millis();

        for (size_t i = 0; i < NUM_DEG; i++)
        {
            data[i] = CData(dsp, c);
        }
    }
};

class LightSection : public Effect
{
protected:
public:
    void DoStuff() override
    {
        _last_time = millis();
    }

    LightSection(double deg, double w, int str, CRGB c) : Effect(c, false)
    {
        for (size_t i = deg - w / 2; i < deg + w / 2; i++)
        {
            if (i >= 0 || i >= 360)
            {
                data[i] = CData(true, c);
                data[i].c.maximizeBrightness();
                data[i].c.fadeLightBy(255 - str);
            }
        }
    }
};

// class Wobble : public Effect
// {
// protected:
//     double _w_freq; //w/second
//     double _w_amount;
//     int _max_wobble;
//     int _med_wobble;
//     float _current_wobble;
//     bool rise = true;

//     double getWobbleAmount()
//     {
//         Blink(OP0, 10);
//         auto w = (256 / _w_amount) * (1000 / ((1000 / _w_freq) / (millis() - _last_time)));
//         int derp = w;
//         //Serial.write(derp);
//         Serial.println(derp, DEC);
//         return w;
//     }

//     int Cap255(int n)
//     {
//         if (n < 0 || n > 255)
//             return 128;
//         return n;
//     }

// public:
//     void DoStuff(int time)
//     {

//         // if (millis() - _last_time > (1000 / _w_freq))
//         // {

//         Blink(OP3, 10);
//         if (rise)
//         {
//             Blink(OP1, 10);
//             if (_current_wobble + getWobbleAmount() >= _med_wobble + _max_wobble)
//             {

//                 rise = false;
//             }

//             _current_wobble += getWobbleAmount();
//         }
//         else
//         {
//             Blink(OP2, 10);
//             if (_current_wobble - getWobbleAmount() <= _med_wobble - _max_wobble)
//             {

//                 rise = true;
//             }
//             _current_wobble -= getWobbleAmount();
//         }

//         for (int i = 0; i < NUM_DEG; ++i)
//         {
//             //data[i].fadeLightBy(5);
//             //data[i] = CRGB::Green;
//             data[i].fadeLightBy(Cap255(_current_wobble));
//         }
//         _last_time = millis();
//     }

//     explicit Wobble(CRGB c, double wtime = 1, double wamount = 1, int wmax = 50, int wmed = 128) : Effect(c)
//     {
//         _last_time = millis();
//         _current_wobble = _med_wobble;
//     }
// };

class EffectHandler
{
private:
    std::vector<Effect *> effects;
    CRGB leds[NUM_LEDS];
    CRGB data[NUM_DEG];
    double r, g, b;
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
        //digitalWrite(OUTPUT_LED_PIN, HIGH);

        int deltaT = millis() - _last_time;
        //update effects
        for (auto e : effects)
        {
            e->DoStuff();
        }
        Blink(OP0);

        int rm = 0, bm = 0, gm = 0;

        //mix effect outputs
        if (effects.size() > 0)
        {
            for (int i = 0; i < NUM_DEG; ++i)
            {
                int rcount = 0;

                if (effects[0]->data[i].doRender)
                {
                    //Blink(OP1);
                    rcount++;
                    r = effects[0]->data[i].c.r;
                    g = effects[0]->data[i].c.g;
                    b = effects[0]->data[i].c.b;

                    // if (rm < effects[0]->data[i].c.r)
                    //     rm = effects[0]->data[i].c.r;
                    // if (gm < effects[0]->data[i].c.g)
                    //     gm = effects[0]->data[i].c.g;
                    // if (bm < effects[0]->data[i].c.b)
                    //     bm = effects[0]->data[i].c.b;
                }
                for (size_t j = 1; j < effects.size(); j++)
                {
                    if (effects[j]->data[i].doRender)
                    {
                        Blink(OP2, 0);
                        rcount++;

                        // if (rm < effects[j]->data[i].c.r)
                        //     rm = effects[j]->data[i].c.r;
                        // if (gm < effects[j]->data[i].c.g)
                        //     gm = effects[j]->data[i].c.g;
                        // if (bm < effects[j]->data[i].c.b)
                        //     bm = effects[j]->data[i].c.b;

                        r = (effects[j]->data[i].c.r);
                        g = (effects[j]->data[i].c.g);
                        b = (effects[j]->data[i].c.b);
                    }
                }
                if (rcount > 0)
                {
                    // r = r / (rcount);
                    // g = g / (rcount);
                    // b = b / (rcount);

                    // r = r + (r_weight * rm) / (rcount + r_weight);
                    // g = g + (r_weight * gm) / (rcount + r_weight);
                    // b = b + (r_weight * bm) / (rcount + r_weight);

                    data[i] = CRGB(r, g, b);
                }
            }

            Blink(OP1);
            for (int i = 0; i < NUM_LEDS; ++i)
            {
                int di = i * 3.75 - 1;

                leds[i] = data[di]; //<- ez tré, csúszó átlag?
                // double sr, sg, sb;
                // bool first = true;
                // int halfWidth = (NUM_DEG / NUM_LEDS + 1) / 2;
                // for (size_t j = -halfWidth; j < halfWidth; j++){}

                //CRGB data[NUM_DEG];
            }
        }
        else
        {
            //Error, random colour in every 2sec
            //auto c = CRGB(rand() % 255, rand() % 255, rand() % 255);
            for (int i = 0; i < NUM_LEDS; ++i)
            {
                leds[i] = CRGB(rand() % 32, rand() % 32, rand() % 64 + 64);
            }
            delay(2000);
        }

        FastLED.show();
        //digitalWrite(OUTPUT_LED_PIN, LOW);
    }

    void Start()
    {

        //Serial.begin(9600);
        pinMode(OP0, OUTPUT);
        pinMode(OP1, OUTPUT);
        pinMode(OP2, OUTPUT);
        pinMode(OP3, OUTPUT);

        Blink(OP0, 100);
        Blink(OP1, 100);
        Blink(OP2, 100);
        Blink(OP3, 100);

        FastLED.addLeds<NEOPIXEL, OUTPUT_PIN>(leds, NUM_LEDS);
        FastLED.setBrightness(brightness);
        FastLED.clear();

        int r = rand() % 255;
        fill_solid(leds, NUM_LEDS, CRGB(r, r, r));

        FastLED.show();

        //  this->AddEffect(new Effect(CRGB::LightSkyBlue));
        //this->AddEffect(new Wobble(CRGB::AliceBlue, 2));
        //this->AddEffect(new LightSection(282, 15, 128, CRGB::OrangeRed));
        //this->AddEffect(new LightSection(192, 15, 192, CRGB::OrangeRed));
        _last_time = millis();
    }
};
