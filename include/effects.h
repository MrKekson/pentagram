#pragma once

#include <FastLED.h>

#include "base.h"
#include "debug.h"

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

int Clamp(int degNum, int clamp = NUM_DEG) // cyrcle clamp
{
    int current = degNum;
    if (degNum < 0)
    {
        current = clamp + degNum;
    }

    if (degNum >= NUM_DEG)
    {
        current = degNum - clamp;
    }
    return current;
}

double Clamp(double degNum, double clamp = NUM_DEG) // cyrcle clamp
{
    double current = degNum;

    if (degNum < 0)
    {
        current = clamp + degNum;
    }

    if (degNum >= NUM_DEG)
    {
        current = degNum - clamp;
    }
    return current;
}

class BaseEffect
{
protected:
    void ResetData()
    {
        CData oneData = CData(0, _c);

        for (size_t i = 0; i < NUM_DEG; i++)
        {
            _data[i] = oneData;
        }
    }

public:
    CHSV _c;
    CData _data[NUM_DEG];
    int _rWeight;
    double _deg;
    double _width;

    int64_t runTime = 0;
    int64_t startTime = -1;
    int64_t endTime = -1;
    int64_t LastRunTime = 0;

    bool isStarted()
    {
        if (startTime < 0)
        {
            return true;
        }

        return startTime < esp_timer_get_time();
    }

    bool isEnded()
    {
        if (endTime < 0)
        {
            return false;
        }
        return endTime < esp_timer_get_time();
    }

    void Render()
    {
        ResetData();

        // for (int i = ((NUM_DEG - 1) - _deg) - _width / 2.0; i < (NUM_DEG - _deg) + _width / 2.0; i++)
        // do we ned reset here, we can set
        if (isEnded() || !isStarted())
        {
            return;
        }
        for (int i = (_deg)-_width / 2.0; i < _deg + _width / 2.0; i++)
        {
            _data[Clamp(i)] = CData(_rWeight, _c);
        }
    }

    virtual void CalcStep(int64_t time)
    {
        // ResetData();
    }

    explicit BaseEffect(CHSV c, double deg, double w, int rw = 0)
    {
        _c = c;
        _deg = deg;
        _width = w;
        _rWeight = rw;

        // _start_time = esp_timer_get_time();
        // _last_time = esp_timer_get_time();

        for (size_t i = 0; i < NUM_DEG; i++)
        {
            _data[i] = CData(_rWeight, _c);
        }
    }
};

// typedef double (*OneValueChanger)(double, int64_t);

using OneValueChanger = std::function<double(double, int64_t)>;
// typedef double (*TwoValueChanger)(double, int64_t);

class SetAngleEffect : public BaseEffect
{
protected:
public:
    OneValueChanger changerStuff;
    // TwoValueChanger changerStuff2;

    void CalcStep(int64_t time) override
    {
        if (changerStuff != nullptr)
        {
            _deg = changerStuff(_deg, time);
        }
        // if(changerStuff2 != nullptr)
        // {
        //     _deg = changerStuff2();
        // }
    }

    SetAngleEffect(CHSV c, double deg, OneValueChanger degChanger, double w, int rw = 128) : BaseEffect(c, deg, w, rw), changerStuff(degChanger)
    {
    }
    // SetAngleEffect(CHSV c, double deg, TwoValueChanger stuffChange2, double w, int rw = 128) : BaseEffect(c, deg, w, rw), changerStuff(stuffChanger2)
    // {
    // }

    double DurationRatio()
    {
        return (double)(esp_timer_get_time() - startTime) / (double)(endTime - startTime);
    }
};

// class RotateSector : public SetAngleEffect
// {
// protected:
//     bool _dir;
//     double _rpm;

// public:
//     void CalcStep() override
//     {
//         _deg = RotateDeg(_deg, _rpm, _dir, _last_time);

//         _last_time = esp_timer_get_time();

//         SetAngleEffect::CalcStep();
//     }

//     RotateSector(CHSV c, double deg, double w, double rpm, int dir, int rw = 128) : SetAngleEffect(c, deg, w, rw)
//     {
//         _c = c;
//         _width = w;
//         _deg = deg;
//         _rpm = rpm;
//         _dir = dir;
//         _rWeight = rw;
//     }
// };

class SetSymbolEffect : public SetAngleEffect
{
protected:
public:
    int _symNum;
    void CalcStep(int64_t time) override
    {
        SetAngleEffect::CalcStep(time);
    }

    SetSymbolEffect(CHSV c, int symbolNumba, int rw = 128) : SetAngleEffect(c, 0, 0, rw)
    {
        _c = c;
        _symNum = symbolNumba;
        _rWeight = rw;

        int newDeg = (_symNum * SYMBOL_WIDTH) + SYMBOL_OFFSET;
        _width = SYMBOL_LED_WIDTH;
        _deg = newDeg;
    }
};

// class Effect
// {
// protected:
//     CHSV _c;
//     unsigned long _start_time;
//     unsigned long _last_time; // last run time to calculate effect amount

//     void ResetData()
//     {
//         CData oneData = CData(0, _c);

//         for (size_t i = 0; i < NUM_DEG; i++)
//         {
//             _data[i] = oneData;
//         }
//     }

//     // void UpdatePart(CHSV c, double posDeg, double width, double str = 0)
//     // {
//     //     if (str == 0)
//     //     {
//     //         str = _rWeight;
//     //     }; // 0 nem lesz jó

//     //     for (int i = posDeg - width / 2; i < posDeg + width / 2; i++)
//     //     {
//     //         _data[Clamp(i)] = CData(str, c);
//     //     }
//     // }

//     // void SetSymbolEffect(CHSV c, int sNum, int str)
//     // {
//     //     int pos = (sNum * SYMBOL_WIDTH) + SYMBOL_OFFSET;
//     //     UpdatePart(c, pos, SYMBOL_LED_WIDTH, str);
//     // }

// public:
//     CData _data[NUM_DEG];
//     int _rWeight;

//     virtual void CalcStep()
//     {
//         _last_time = millis();
//     }

//     virtual void SetColor(CHSV c)
//     {
//         _c = c;
//     }

//     explicit Effect(CHSV c, int rw = 0)
//     {
//         _start_time = millis();
//         _c = c;
//         _rWeight = rw;
//         _last_time = millis();

//         for (size_t i = 0; i < NUM_DEG; i++)
//         {
//             _data[i] = CData(_rWeight, _c);
//         }
//     }
// };

// class SymbolSimpleFade : public Effect
// {
// protected:
//     int _sNum;
//     long _duration;

// public:
//     void CalcStep() override
//     {
//         ResetData();
//         SetSymbolEffect(_c, _sNum, _rWeight);
//         // if (millis() < _duration + _start_time)
//         // {
//         //     // DLED::Blink(3, 10);
//         //     long deltaW = (_rWeight / _duration) * (millis() - _start_time);

//         //     // auto c = CHSV(_c.h + deltaC, _c.s + deltaC, _c.v + deltaC);

//         //     ResetData();

//         //     UpdatePart(_c, _deg, _width, deltaW);
//         //     _last_time = millis();
//         // }
//     }

//     // should be symbol numba
//     SymbolSimpleFade(CHSV c, int sNum, long dur, int rw = 128) : Effect(c, 0)
//     {
//         _c = c;
//         _sNum = sNum;
//         _duration = dur;
//         _rWeight = rw;
//         _last_time = millis();
//         _start_time = millis();
//     }
// };

// class DeepLightEffect : public Effect
// {
// public:
//     void CalcStep() override
//     {
//         _last_time = millis();
//     }

//     DeepLightEffect(CHSV c, int deg = 30, int width = LED_RATIO, int degShift = 0, int rw = 128) : Effect(c, 0)
//     {
//         _rWeight = rw;
//         _last_time = millis();

//         ResetData();

//         for (int i = 0; i < NUM_DEG; i += deg)
//         {
//             for (int j = i - width / 2; j < i + width / 2; j++)
//             {
//                 int curr = Clamp(j + degShift);
//                 _data[curr] = CData(_rWeight, c);
//             }
//         }
//     }
// };

// class LightSection : public Effect
// {
// protected:
//     double _deg;
//     double _width;
//     bool _dir;
//     double _speed;

// public:
//     void CalcStep() override
//     {
//         _last_time = millis();
//         if (_dir)
//         {
//             _deg += _speed;
//         }
//         else
//         {
//             _deg -= _speed;
//         }

//         if (_deg >= 360)
//         {
//             _deg = 0;
//         }
//         if (_deg < 0)
//         {
//             _deg = 360;
//         }
//         ResetData();

//         UpdatePart(_c, _deg, _width);
//     }

//     LightSection(double deg, double w, double s, bool forward, CHSV c, int rw = 128) : Effect(c, 0)
//     {
//         _c = c;
//         _width = w;
//         _deg = deg;
//         _speed = s;
//         _dir = forward;
//         _rWeight = rw;
//     }
// };

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
//     void CalcStep(int time)
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