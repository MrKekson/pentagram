#include <FastLED.h>
#include <vector.h>

#include "effects.h"

double SymbolToDeg(int numba)
{
    if (numba < 1 || numba > 12)
    {
        return 0;
    }

    double pos = ((numba - 1) * SYMBOL_WIDTH) + SYMBOL_OFFSET;
    return pos;
}

int DegToSymbol(double numba)
{
    double pos = int((numba - SYMBOL_OFFSET) / SYMBOL_WIDTH) + 1;
    return pos;
}

template <class T>
using ValueChanger = std::function<T(T, int64_t)>;

template <class T>
ValueChanger<T> createLinearValueChanger(T start, T end, int64_t duration)
{
    return [start, end, duration](T currentValue, int64_t deltaT)
    {
        auto progress = (double)deltaT / (double)duration;
        auto step = (end - start) * (double)progress;
        return start + step;
    };
}

std::vector<BaseEffect *> Trickle()
{
    std::vector<BaseEffect *> effects;

    auto startTime = 0 * SEC_TO_MICRO;
    auto endTime = 12 * SEC_TO_MICRO;
    auto duration = endTime - startTime;

    auto maxWeight = 128;
    auto minWeight = 0;

    auto baseColorEffect = new SetAngleEffect(CHSV(160, 64, 64), nullptr, 0, nullptr, 360, nullptr, maxWeight, nullptr, startTime, endTime);

    effects.push_back(baseColorEffect);

    int symbolFrom = 2, symbolTo = 7;

    auto color = CHSV(140, 200, 200);
    auto deg = 90;
    auto width = 50; // degree

    auto sDeg = SymbolToDeg(2);
    auto weightLambda = createLinearValueChanger(maxWeight, minWeight, duration);
    auto effect = new SetAngleEffect(color, nullptr, sDeg, nullptr, SYMBOL_LED_WIDTH, nullptr, maxWeight, weightLambda, startTime, endTime);
    effects.push_back(effect);

    auto sDeg2 = SymbolToDeg(7);
    auto weightLambda2 = createLinearValueChanger(minWeight, maxWeight, duration);
    auto effect2 = new SetAngleEffect(color, nullptr, sDeg2, nullptr, SYMBOL_LED_WIDTH, nullptr, minWeight, weightLambda2, startTime, endTime);
    effects.push_back(effect2);

    int rnd = 5 + rand() % 10;

    int trickleWeight = 90;
    int trickleWidth = 3;

    for (size_t i = 0; i < rnd; i++)
    {
        auto dir = rand() % 2;
        auto timeRange = (endTime / SEC_TO_MICRO) / 2;

        auto start = (rand() % timeRange) * SEC_TO_MICRO;
        auto stop = endTime - ((rand() % timeRange) * SEC_TO_MICRO);
        auto duration = stop - start;

        if (dir == 0)
        {
            auto degChanger = createLinearValueChanger(sDeg, sDeg2, duration);
            auto effect2 = new SetAngleEffect(color, nullptr, sDeg, degChanger, trickleWidth, nullptr, trickleWeight, nullptr, start, stop);
            effects.push_back(effect2);
        }
        else
        {
            // auto nDeg = sDeg - sDeg2;
            auto degChanger = createLinearValueChanger(sDeg, -sDeg2, duration);
            auto effect2 = new SetAngleEffect(color, nullptr, sDeg, degChanger, trickleWidth, nullptr, trickleWeight, nullptr, start, stop);
            effects.push_back(effect2);
        }
    }

    return effects;
}

std::vector<BaseEffect *> createRotate300()
{
    std::vector<BaseEffect *> effects;

    auto color = CHSV(190, 255, 255);
    auto deg = 90;
    auto width = 50; // degree
    int strength = 128;

    auto baseColorEffect = new SetAngleEffect(CHSV(180, 128, 96), nullptr, 0, nullptr, 360, nullptr, 0, nullptr);
    effects.push_back(baseColorEffect);

    auto startTime = 1 * SEC_TO_MICRO;
    auto endTime = 5 * SEC_TO_MICRO;
    auto duration = endTime - startTime;
    auto lambda = [duration](double D, int64_t deltaT)
    {
        auto progress = (double)deltaT / (double)duration;
        auto step = -300.0 * (double)progress;
        return Clamp(step + 60);
    };

    auto effect = new SetAngleEffect(
        color,
        nullptr,
        60,
        lambda,
        width,
        lambda,
        strength,
        nullptr,
        startTime,
        endTime);

    effects.push_back(effect);

    return effects;
}

std::vector<BaseEffect *> createEgymassalSzembeForog()
{
    std::vector<BaseEffect *> effects;

    auto color = CHSV(190, 150, 150);
    auto effect = new SetAngleEffect(
        color, nullptr, 0, [](double D, int64_t deltaT)
        { return Clamp(D + 1); },
        5, nullptr, 255, nullptr, 5 * SEC_TO_MICRO, 25 * SEC_TO_MICRO);

    effects.push_back(effect);

    auto effect2 = new SetAngleEffect(
        color, nullptr, Clamp(180.0), [](double D, int64_t deltaT)
        { return Clamp(D + 1); },
        5, nullptr, 255, nullptr, 5 * SEC_TO_MICRO, 25 * SEC_TO_MICRO);

    effects.push_back(effect2);

    return effects;
}
