#pragma once
#include <FastLED.h>

#include "output.h"

#define NUM_DEG 360

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

class Effect
{
protected:
    CHSV _c;
    int _last_time;

public:
    CData _data[NUM_DEG];
    int _rWeight;

    virtual void DoStuff()
    {
        _last_time = millis();
    }

    virtual void SetColor(CHSV c)
    {
        _c = c;
    }

    explicit Effect(CHSV c, int rw = 0)
    {
        _c = c;
        _rWeight = rw;
        _last_time = millis();

        for (size_t i = 0; i < NUM_DEG; i++)
        {
            _data[i] = CData(_rWeight, _c);
        }
    }
};

class DeepLightEffect : public Effect
{
public:
    void DoStuff() override
    {
        _last_time = millis();
    }

    DeepLightEffect(CHSV c, int rw = 128) : Effect(c, 0)
    {
        _rWeight = rw;
        _last_time = millis();

        for (size_t i = 7; i <= NUM_DEG - 8; i += 5)
        {
            //_data[i] = CData(_rWeight, c);
        }
    }
};

class LightSection : public Effect
{
protected:
    double _deg;
    double _width;
    bool _dir;
    double _speed;

public:
    void DoStuff() override
    {
        _last_time = millis();
        if (_dir)
        {
            _deg += _speed;
        }
        else
        {
            _deg -= _speed;
        }

        if (_deg >= 360)
        {
            _deg = 0;
        }

        if (_deg < 0)
        {
            _deg = 360;
        }

        for (size_t i = 0; i < NUM_DEG; i++)
        {
            _data[i].rWeight = 0;
        }

        for (int i = _deg - _width / 2; i < _deg + _width / 2; i++)
        {
            if (i < 0)
            {
                _data[360 + i] = CData(_rWeight, _c);
            }

            if (i >= 0 && i < 360)
            {
                _data[i] = CData(_rWeight, _c);
            }

            if (i >= 360)
            {
                _data[i - 360] = CData(_rWeight, _c);
            }
        }
    }

    LightSection(double deg, double w, double s, bool forward, CHSV c, int rw = 128) : Effect(c, 0)
    {
        _c = c;
        _width = w;
        _deg = deg;
        _speed = s;
        _dir = forward;
        _rWeight = rw;

        for (size_t i = deg - w / 2; i < deg + w / 2; i++)
        {
            if (i < 0)
            {
                _data[360 + i] = CData(_rWeight, _c);
            }

            if (i >= 0 && i < 360)
            {
                _data[i] = CData(_rWeight, _c);
            }

            if (i >= 360)
            {
                _data[i - 360] = CData(_rWeight, _c);
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
//             //_data[i].fadeLightBy(5);
//             //_data[i] = CRGB::Green;
//             _data[i].fadeLightBy(Cap255(_current_wobble));
//         }
//         _last_time = millis();
//     }

//     explicit Wobble(CRGB c, double wtime = 1, double wamount = 1, int wmax = 50, int wmed = 128) : Effect(c)
//     {
//         _last_time = millis();
//         _current_wobble = _med_wobble;
//     }
// };