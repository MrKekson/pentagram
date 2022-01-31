#pragma once

#include "base.h"
#include "BaseEffect.h"

double RotateDeg(double _deg, double _rpm, int dir, int64_t _last_time)
{
        int64_t timePassed = esp_timer_get_time() - _last_time;
        double rotAngleInTime = ((double)timePassed / ((double)MICROS_IN_MINUTE / _rpm)) * (double)NUM_DEG;

        if(dir >= 0)
        {
            _deg += rotAngleInTime;
        } else
        {
            _deg -= rotAngleInTime;
        }

        // Serial.print(" ---- ");
        // Serial.print(timePassed, 12);
        // Serial.print(" ---- ");
        // Serial.print(((double)timePassed / ((double)MICROS_IN_MINUTE / _rpm)) * (double)NUM_DEG, 12);
        // Serial.print(" ---- ");
        // Serial.print(rotAngleInTime, 12);
        // Serial.print(" ---- ");
        // Serial.print(_deg);

        _deg = Clamp(_deg);

        // Serial.print(" ---- ");
        // Serial.print(_deg);
        // Serial.println();
        return _deg;
}

class UpdateSector : public BaseEffect
{
protected:
    

public:
    double _deg;
    double _width;

    void DoStuff() override
    {
        ResetData();  

        //for (int i = ((NUM_DEG - 1) - _deg) - _width / 2.0; i < (NUM_DEG - _deg) + _width / 2.0; i++)

        for (int i = ( _deg) - _width / 2.0; i < _deg + _width / 2.0; i++)
        {
            _data[Clamp(i)] = CData(_rWeight, _c);
        }
    }

    UpdateSector(CHSV c, double deg, double w, int rw = 128) : BaseEffect(c, 0)
    {
        _c = c;
        _width = w;
        _deg = deg;
        _rWeight = rw;
    }
};

typedef double(*degCalcFunctionType)(double);

class UpdateSector2 : public BaseEffect
{
protected:
    

public:
    double _deg;
    double _width;
    degCalcFunctionType degcalculator;

    void DoStuff() override
    {
        ResetData();

        degcalculator(12);

        for (int i = ((NUM_DEG - 1) - _deg) - _width / 2.0; i < (360 - _deg) + _width / 2.0; i++)
        {
            _data[Clamp(i)] = CData(_rWeight, _c);
        }
    }

    void setFg(degCalcFunctionType funcPointer){}

    UpdateSector2(CHSV c, degCalcFunctionType funcPointer, double w, int rw = 128) : BaseEffect(c, 0)
    {
        degcalculator = funcPointer;
        _c = c;
        _width = w;
        _rWeight = rw;
    }
};

class RotateSector : public UpdateSector
{
protected:
    bool _dir;
    double _rpm;

public:
    void DoStuff() override
    {
        _deg = RotateDeg(_deg, _rpm, _dir, _last_time);

        _last_time = esp_timer_get_time();

        UpdateSector::DoStuff();
    }

    RotateSector(CHSV c, double deg, double w, double rpm, int dir, int rw = 128) : UpdateSector(c, deg, w, rw)
    {
        _c = c;
        _width = w;
        _deg = deg;
        _rpm = rpm;
        _dir = dir;
        _rWeight = rw;
    }
};

class SetSymbol : public UpdateSector
{
protected:
    int _symNum;

public:
    void DoStuff() override
    {       
        UpdateSector::DoStuff();
    }

    SetSymbol(CHSV c, int symbolNumba, int rw = 128) : UpdateSector(c, 0, 0, rw)
    {
        _c = c;
        _symNum = symbolNumba;
        _rWeight = rw;

         int newDeg = (_symNum * SYMBOL_WIDTH) + SYMBOL_OFFSET;
        _width = SYMBOL_LED_WIDTH;
        _deg = newDeg;
    }
};
