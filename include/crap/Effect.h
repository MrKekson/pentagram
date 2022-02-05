// #pragma once

// #include <FastLED.h>

// #include "base.h"
// #include "debug.h"

// struct CData
// {
//     CData()
//     {
//         rWeight = 0;
//         c = CHSV(0, 0, 0);
//     }
//     CData(int rw, CHSV col)
//     {
//         rWeight = rw;
//         c = CHSV(col);
//     }
//     int rWeight = 0; /*render weight*/
//     CHSV c;          /* _data */
// };

// int Clamp(int degNum, int clamp = NUM_DEG)
// {
//     int current = degNum;
//     if (degNum < 0)
//     {
//         current = clamp + degNum; // = CData(ww, c);
//     }

//     if (degNum >= NUM_DEG)
//     {
//         current = degNum - clamp; // = CData(ww, c);
//     }
//     return current;
// }

// double Clamp(double degNum, double clamp = NUM_DEG)
// {
//     double current = degNum;
//     if (degNum < 0)
//     {
//         current = clamp + degNum; // = CData(ww, c);
//     }

//     if (degNum >= NUM_DEG)
//     {
//         current = degNum - clamp; // = CData(ww, c);
//     }
//     return current;
// }

// class BaseEffect
// {
// protected:
//     void ResetData()
//     {
//         CData oneData = CData(0, _c);

//         for (size_t i = 0; i < NUM_DEG; i++)
//         {
//             _data[i] = oneData;
//         }
//     }

// public:
//     CHSV _c;
//     CData _data[NUM_DEG];
//     int _rWeight;
//     double _deg;
//     double _width;

//     int64_t runTime = 0;
//     int64_t startTime = 0;
//     int64_t endTime = 0;
//     int64_t duration = 0;

//     bool isStarted = false;
//     bool isEnded = false;

//     virtual void CalcStep()
//     {
//         // ResetData();
//     }

//     explicit BaseEffect(CHSV c, double deg, double w, int rw = 0)
//     {
//         _c = c;
//         _deg = deg;
//         _width = w;
//         _rWeight = rw;

//         // _start_time = esp_timer_get_time();
//         // _last_time = esp_timer_get_time();

//         for (size_t i = 0; i < NUM_DEG; i++)
//         {
//             _data[i] = CData(_rWeight, _c);
//         }
//     }
// };

// // EFFECT EXAMPLE CODE

// // #pragma once

// // #include "BaseEffect.h"

// // class CustomSector : public BaseEffect
// // {
// // protected:
// //     int _sNum;
// //     long _duration;

// // public:
// //     void CalcStep() override
// //     {
// //         BaseEffect::CalcStep();  //this has the reset called

// // yourstuff
// //     }

// //     // should be symbol numba
// //     CustomSector(CHSV c, int sNum, long dur, int rw = 128) : BaseEffect(c, 0)
// //     {
// //         _c = c;
// //         _sNum = sNum;
// //         _duration = dur;
// //         _rWeight = rw;
// //         _last_time = millis();
// //         _start_time = millis();
// //     }
// // };