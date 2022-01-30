#pragma once

#include "base.h"
#include "BaseEffect.h"

class UpdateSector : public BaseEffect
{
protected:
    double _deg;
    double _width;

public:
    void DoStuff() override
    {
        ResetData();  

        for (int i = ((NUM_DEG - 1) - _deg) - _width / 2.0; i < (360 - _deg) + _width / 2.0; i++)
        {
            _data[Clamp(i)] = CData(_rWeight, _c);
        }
    }

    // should be symbol numba
    UpdateSector(CHSV c, double deg, double w, int rw = 128) : BaseEffect(c, 0)
    {
        _c = c;
        _width = w;
        _deg = deg;
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
        int64_t timePassed = esp_timer_get_time() - _last_time;
        double rotAngleInTime = ((double)timePassed / ((double)MICROS_IN_MINUTE / _rpm)) * (double)NUM_DEG;

        if(_dir)
        {
            _deg += rotAngleInTime;
        } else
        {
            _deg -= rotAngleInTime;
        }

        Serial.print(" ---- ");
        Serial.print(timePassed, 12);
        Serial.print(" ---- ");
        Serial.print(((double)timePassed / ((double)MICROS_IN_MINUTE / _rpm)) * (double)NUM_DEG, 12);
        Serial.print(" ---- ");
        Serial.print(rotAngleInTime, 12);
        Serial.print(" ---- ");
        Serial.print(_deg);


        _deg = Clamp(_deg);

        Serial.print(" ---- ");
        Serial.print(_deg);
        Serial.println();
                
        _last_time = esp_timer_get_time();

        UpdateSector::DoStuff();
    }

    // 
    RotateSector(CHSV c, double deg, double w, double rpm, bool forward, int rw = 128) : UpdateSector(c, deg, w, rw)
    {
         _c = c;
        _width = w;
        _deg = deg;
        _rpm = rpm;
        _dir = forward;
        _rWeight = rw;
    }
};

//void RotateDeg(double deg, unsigned long lastTime, unsigned long speed) 
// {
//     auto currentTime = millis();
// }