// #pragma once

// #include "base.h"
// #include "effects.h"

// double RotateDeg(double _deg, double _rpm, int dir, int64_t _last_time)
// {
//     int64_t timePassed = esp_timer_get_time() - _last_time;
//     double rotAngleInTime = ((double)timePassed / ((double)MICROS_IN_MINUTE / _rpm)) * (double)NUM_DEG;

//     if (dir >= 0)
//     {
//         _deg += rotAngleInTime;
//     }
//     else
//     {
//         _deg -= rotAngleInTime;
//     }

//     // Serial.print(" ---- ");
//     // Serial.print(timePassed, 12);
//     // Serial.print(" ---- ");
//     // Serial.print(((double)timePassed / ((double)MICROS_IN_MINUTE / _rpm)) * (double)NUM_DEG, 12);
//     // Serial.print(" ---- ");
//     // Serial.print(rotAngleInTime, 12);
//     // Serial.print(" ---- ");
//     // Serial.print(_deg);

//     _deg = Clamp(_deg);

//     // Serial.print(" ---- ");
//     // Serial.print(_deg);
//     // Serial.println();
//     return _deg;
// }
