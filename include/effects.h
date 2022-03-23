#pragma once

#include <FastLED.h>
#include <limits.h>
#include "base.h"
#include "debug.h"

template <class T>
using ValueChanger = std::function<T(T, int64_t)>;

using DoubleValueChanger = std::function<double(double, int64_t)>;
using ColorValueChanger = std::function<CHSV(CHSV, int64_t)>;
using IntValueChanger = std::function<int(int, int64_t)>;
// typedef double (*TwoValueChanger)(double, int64_t);

class BaseEffect
{
protected:
    bool isStarted(int64_t now)
    {
        return startTime <= now;
    }

    bool isEnded(int64_t now)
    {
        return endTime < now;
    }

public:
    CHSV _c;
    double _deg;
    double _width;
    int _rWeight;

    bool isFullRenderTime;

    int64_t runTime;
    int64_t animationStartTime;
    int64_t startTime;
    int64_t endTime;
    int64_t LastRunTime;

    int64_t localStartTime;
    int64_t localEndTime;

    ColorValueChanger colorChanger;
    DoubleValueChanger degreeChanger;
    DoubleValueChanger widthChanger;
    IntValueChanger weightChanger;

    void setAnimationStartTime(int64_t now)
    {
        animationStartTime = now;
        startTime = now + localStartTime;
        endTime = now + localEndTime;
    }

    bool isRunning(int64_t now)
    {
        if (isStarted(now) && !isEnded(now))
        {
            return true;
        }
        return false;
    }

    void CalcStep(int64_t now)
    {
        if (isRunning(now))
        {
            int64_t deltaT = now - startTime;
            if (colorChanger != nullptr)
            {
                _c = colorChanger(_c, deltaT);
            }
            if (degreeChanger != nullptr)
            {
                _deg = degreeChanger(_deg, deltaT);
            }
            if (widthChanger != nullptr)
            {
                _width = widthChanger(_width, deltaT);
            }
            if (weightChanger != nullptr)
            {
                _rWeight = weightChanger(_rWeight, deltaT);
            }
        }
    }

    BaseEffect(
        CHSV c,
        ColorValueChanger colorChanger,
        double deg,
        DoubleValueChanger degChanger,
        double w,
        DoubleValueChanger widthChanger,
        int rw,
        IntValueChanger weightChanger,
        int64_t st = 0,
        int64_t et = MAX_ANIM_LENGHT,
        bool isFRender = 0)
        : _c(c),
          _deg(deg),
          _width(w),
          _rWeight(rw),
          isFullRenderTime(isFRender),
          localStartTime(st),
          localEndTime(et),
          colorChanger(colorChanger),
          degreeChanger(degChanger),
          widthChanger(widthChanger),
          weightChanger(weightChanger)
    {
    }
};
