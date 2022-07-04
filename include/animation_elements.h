#pragma once

#include <FastLED.h>
#include <vector.h>

#include "effects.h"
#include "base.h"

template <class T>
using ValueChanger = std::function<T(T, int64_t)>;
using ColorChanger = std::function<DCHSV(DCHSV, int64_t)>;

// template <class T>
ColorChanger createLinearColorChanger(DCHSV valStart, DCHSV valEnd, int64_t duration)
{
    return [valStart, valEnd, duration](DCHSV currentValue, int64_t deltaT)
    {
        double hs = valStart.H, ss = valStart.S, vs = valStart.V;
        double he = valEnd.H, se = valEnd.S, ve = valEnd.V;

        double hd = he - hs, sd = se - ss, vd = ve - vs;

        double progress = (double)deltaT / (double)duration;

        double hstep = hd * (double)progress;
        double sstep = sd * (double)progress;
        double vstep = vd * (double)progress;

        DCHSV retCol = DCHSV(valStart.H + hstep, valStart.S + sstep, valStart.V + vstep);

        return retCol; // + step;
    };
}

template <class T>
ValueChanger<T> createLinearValueChanger(T valStart, T valEnd, int64_t duration)
{
    return [valStart, valEnd, duration](T currentValue, int64_t deltaT)
    {
        T valueChangeRange = (valEnd - valStart);

        double progress = (double)deltaT / (double)duration;
        T step = valueChangeRange * (double)progress;
        return valStart + step;
    };
}

template <class T>
ValueChanger<T> createFlickerValueChanger(T valStart, T valEnd, int64_t duration)
{
    return [valStart, valEnd, duration](T currentValue, int64_t deltaT)
    {
        T valueChangeRange = (valEnd - valStart);

        double progress = (double)deltaT / (double)duration;
        T step = valueChangeRange * (double)progress;
        return valStart + step;
    };
}

template <class T>
ValueChanger<T> createSinusValueChanger(T valStart, T valEnd, int64_t duration)
{
    return [valStart, valEnd, duration](T currentValue, int64_t deltaT)
    {
        T valueChangeRange = (valEnd - valStart);
        double progress = (double)deltaT / (double)duration;

        T step = valueChangeRange * sin((double)progress * (M_PI));
        return valStart + step;
    };
}

template <class T>
ValueChanger<T> createBezierValueChanger(T valStart, T valEnd, int64_t duration)
{
    return [valStart, valEnd, duration](T currentValue, int64_t deltaT)
    {
        T valueChangeRange = (valEnd - valStart);
        double progress = (double)deltaT / (double)duration;

        T step = valueChangeRange * (progress * progress * (3.0f - 2.0f * progress));
        return valStart + step;
    };
}

template <class T>
ValueChanger<T> GetSpecificValueChanger(int type, T valStart, T valEnd, int64_t duration)
{
    if (type < 0)
    {
        type = rand() % 3;
    }

    switch (type)
    {
    case 0:
        return createLinearValueChanger(valStart, valEnd, duration);
        break;
    case 1:
        return createBezierValueChanger(valStart, valEnd, duration);
        break;
    case 2:
        return createSinusValueChanger(valStart, valEnd, duration);
        break;

    default:
        break;
    }
}

std::vector<BaseEffect *> LightAndHold(SData start, SData transition, int64_t startTime, int64_t endTime, int holdType = 0)
{
    std::vector<BaseEffect *> effects;

    DCHSV color = start.c;
    int sDeg = SymbolToDeg(start.symbol);

    int64_t halfDuration = (endTime - startTime) / 2;

    ValueChanger<int> weightChanger = GetSpecificValueChanger(0, start.weight, transition.weight, halfDuration);

    BaseEffect *effect = new BaseEffect(color, nullptr, sDeg, nullptr, SYMBOL_LED_WIDTH, nullptr, start.weight, weightChanger, startTime, endTime - halfDuration + 200);
    effects.push_back(effect);

    ValueChanger<int> weightChanger2 = GetSpecificValueChanger(0, transition.weight, start.weight, halfDuration);
    BaseEffect *effect2 = new BaseEffect(color, nullptr, sDeg, nullptr, SYMBOL_LED_WIDTH, nullptr, transition.weight, weightChanger2, startTime + halfDuration - 200, endTime);
    effects.push_back(effect2);

    return effects;
}

std::vector<BaseEffect *> FadeTo(SData startData, SData endData, int64_t startTime, int64_t endTime)
{
    std::vector<BaseEffect *> effects;

    {
        int symbolFrom = startData.symbol;
        int symbolTo = endData.symbol;

        int64_t duration = endTime - startTime;
        int64_t halfDuration = duration / 2;

        int maxWeight = startData.weight;
        int minWeight = 0;

        CHSV color = startData.c;

        ColorChanger colChanger = createLinearColorChanger(startData.c, endData.c, duration);

        double sDeg = SymbolToDeg(symbolFrom);
        ValueChanger<int> lambda = createLinearValueChanger(maxWeight, minWeight, halfDuration);
        BaseEffect *effect = new BaseEffect(color, colChanger, sDeg, nullptr, SYMBOL_LED_WIDTH, nullptr, maxWeight, lambda, startTime, endTime - halfDuration);
        effects.push_back(effect);

        double sDeg2 = SymbolToDeg(symbolTo);
        ValueChanger<int> lambda2 = createLinearValueChanger(minWeight, maxWeight, halfDuration);
        BaseEffect *effect2 = new BaseEffect(color, colChanger, sDeg2, nullptr, SYMBOL_LED_WIDTH, nullptr, minWeight, lambda2, startTime + halfDuration, endTime);
        effects.push_back(effect2);
    }
    return effects;
}

std::vector<BaseEffect *> Trickle(SData startData, SData endData, int64_t startTime, int64_t endTime)
{
    std::vector<BaseEffect *> effects;

    {
        int symbolFrom = startData.symbol;
        int symbolTo = endData.symbol;

        int64_t duration = endTime - startTime;

        int64_t delayTime = 1 * SEC_TO_MICRO; // duration/something maybe

        int maxWeight = startData.weight;
        int minWeight = 0;

        CHSV color = startData.c;

        ColorChanger colChanger = createLinearColorChanger(startData.c, endData.c, duration);

        double sDeg = SymbolToDeg(symbolFrom);
        ValueChanger<int> weightLambda = createLinearValueChanger(maxWeight, minWeight, duration - delayTime);
        BaseEffect *effect = new BaseEffect(color, colChanger, sDeg, nullptr, SYMBOL_LED_WIDTH, nullptr, maxWeight, weightLambda, startTime, endTime - delayTime);
        effects.push_back(effect);

        double sDeg2 = SymbolToDeg(symbolTo);
        ValueChanger<int> weightLambda2 = createLinearValueChanger(minWeight, maxWeight, duration - delayTime);
        BaseEffect *effect2 = new BaseEffect(color, colChanger, sDeg2, nullptr, SYMBOL_LED_WIDTH, nullptr, minWeight, weightLambda2, startTime + delayTime, endTime);
        effects.push_back(effect2);
        // Serial.print("Trk HALF ");
        int rnd = 15 + rand() % 85;

        int trickleWeight = (maxWeight / rnd) + 64;
        int trickleWidth = 2;

        for (size_t i = 0; i < rnd; i++)
        {
            int dir = rand() % 2;
            int64_t timeRange = (endTime / SEC_TO_MICRO) - 1;

            int64_t start = (rand() % (timeRange * 5)) * SEC_TO_MICRO / 5;

            int64_t stop = endTime - ((rand() % ((timeRange - start / SEC_TO_MICRO) * 5)) * SEC_TO_MICRO / 5);
            int64_t duration = stop - start;

            if (dir == 1)
            {
                ValueChanger<double> degChanger = createBezierValueChanger(sDeg, sDeg2, duration);
                BaseEffect *tEffect = new BaseEffect(color, colChanger, sDeg, degChanger, trickleWidth, nullptr, trickleWeight, nullptr, start, stop);
                effects.push_back(tEffect);
            }
            else
            {
                int _sDeg = sDeg, _sDeg2 = sDeg2 - NUM_DEG;
                if (sDeg > sDeg2)
                {
                    _sDeg = sDeg - NUM_DEG;
                    _sDeg2 = sDeg2;
                }
                ValueChanger<double> degChanger = createLinearValueChanger(_sDeg, _sDeg2, duration);
                BaseEffect *tEffect = new BaseEffect(color, colChanger, sDeg, degChanger, trickleWidth, nullptr, trickleWeight, nullptr, start, stop);
                effects.push_back(tEffect);
            }
        }
    }
    // Serial.print("   ++++++++ AC FREEHEAP:");
    // Serial.print(ESP.getFreeHeap());
    // Serial.print("\n");

    // Serial.print("Trk Bup \n ");
    return effects;
}

std::vector<BaseEffect *> RotateTo(SData startData, SData endData, int64_t startTime, int64_t endTime)
{
    std::vector<BaseEffect *> effects;

    {
        int symbolFrom = startData.symbol;
        int symbolTo = endData.symbol;

        int64_t duration = endTime - startTime;

        Serial.print(" t: ");
        Serial.print(duration);

        double sDeg = SymbolToDeg(symbolFrom);
        double sDeg2 = SymbolToDeg(symbolTo);

        int dir = rand() % 2;

        ColorChanger colChanger = createLinearColorChanger(startData.c, endData.c, duration);

        if (dir == 1)
        {
            ValueChanger<double> degChanger = createBezierValueChanger(sDeg, sDeg2, duration);
            BaseEffect *tEffect = new BaseEffect(startData.c, colChanger, sDeg, degChanger, SYMBOL_LED_WIDTH, nullptr, startData.weight, nullptr, startTime, endTime);
            effects.push_back(tEffect);
        }
        else
        {
            int _sDeg = sDeg, _sDeg2 = sDeg2 - NUM_DEG;
            if (sDeg > sDeg2)
            {
                _sDeg = sDeg - NUM_DEG;
                _sDeg2 = sDeg2;
            }
            ValueChanger<double> degChanger = createLinearValueChanger(_sDeg, _sDeg2, duration);
            BaseEffect *tEffect = new BaseEffect(startData.c, colChanger, sDeg, degChanger, SYMBOL_LED_WIDTH, nullptr, startData.weight, nullptr, startTime, endTime);
            effects.push_back(tEffect);
        }
    }
    // Serial.print("   ++++++++ AC FREEHEAP:");
    // Serial.print(ESP.getFreeHeap());
    // Serial.print("\n");

    // Serial.print("Trk Bup \n ");
    return effects;
}