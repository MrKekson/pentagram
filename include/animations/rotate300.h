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
ValueChanger<T> createLinearValueChanger(T valStart, T valEnd, int64_t duration)
{
    return [valStart, valEnd, duration](T currentValue, int64_t deltaT)
    {
        T oneStep;
        if ((valStart >= 0 && valEnd >= 0) || (valStart < 0 && valEnd < 0))
        {
            oneStep = (valEnd - valStart);
        }
        else
        {
            oneStep = (valEnd - valStart);
        }

        double progress = (double)deltaT / (double)duration;
        auto step = oneStep * (double)progress;
        return valStart + step;
    };
}

std::vector<BaseEffect *> JustBase()
{
    std::vector<BaseEffect *> effects;

    int64_t startTime = 0 * SEC_TO_MICRO;
    int64_t endTime = 3 * SEC_TO_MICRO;
    SetAngleEffect *baseColorEffect = new SetAngleEffect(CHSV(160, 64, 64), nullptr, 0, nullptr, 360, nullptr, 128, nullptr, startTime - 10000, endTime);
    effects.push_back(baseColorEffect);

    return effects;
}

std::vector<BaseEffect *> Trickle()
{
    Serial.print("Trk Bip ");
    std::vector<BaseEffect *> effects;

    int startTime = 0 * SEC_TO_MICRO;
    int endTime = 3 * SEC_TO_MICRO;
    int delayTime = 1 * SEC_TO_MICRO;

    int duration = endTime - startTime;

    int maxWeight = 256;
    int minWeight = 0;

    SetAngleEffect *baseColorEffect = new SetAngleEffect(CHSV(160, 64, 64), nullptr, 0, nullptr, 360, nullptr, maxWeight, nullptr, startTime, endTime);

    effects.push_back(baseColorEffect);

    int symbolFrom = 2, symbolTo = 9;

    CHSV color = CHSV(140, 200, 200);
    int deg = 90;
    int width = 50; // degree

    double sDeg = SymbolToDeg(symbolFrom);
    auto weightLambda = createLinearValueChanger(maxWeight, minWeight, duration - delayTime);
    SetAngleEffect *effect = new SetAngleEffect(color, nullptr, sDeg, nullptr, SYMBOL_LED_WIDTH, nullptr, maxWeight, weightLambda, startTime, endTime - delayTime);
    effects.push_back(effect);

    double sDeg2 = SymbolToDeg(symbolTo);
    auto weightLambda2 = createLinearValueChanger(minWeight, maxWeight, duration - delayTime);
    SetAngleEffect *effect2 = new SetAngleEffect(color, nullptr, sDeg2, nullptr, SYMBOL_LED_WIDTH, nullptr, minWeight, weightLambda2, startTime + delayTime, endTime);
    effects.push_back(effect2);
    Serial.print("Trk HALF ");
    int rnd = 70; // + rand() % 75;

    int trickleWeight = 70;
    int trickleWidth = 4;

    Serial.print("+++++++BC FREEHEAP:");
    Serial.print(ESP.getFreeHeap());

    for (size_t i = 0; i < rnd; i++)
    {
        int dir = rand() % 2;
        int timeRange = (endTime / SEC_TO_MICRO) - 1;

        int start = (rand() % (timeRange * 5)) * SEC_TO_MICRO / 5;

        int stop = endTime - ((rand() % ((timeRange - start / SEC_TO_MICRO) * 5)) * SEC_TO_MICRO / 5);
        int duration = stop - start;

        if (dir == 0)
        {
            auto degChanger = createLinearValueChanger(sDeg, sDeg2, duration);
            SetAngleEffect *tEffect = new SetAngleEffect(color, nullptr, sDeg, degChanger, trickleWidth, nullptr, trickleWeight, nullptr, start, stop);
            effects.push_back(tEffect);
        }
        else
        {

            auto degChanger = createLinearValueChanger(sDeg, sDeg2 - NUM_DEG, duration);
            SetAngleEffect *tEffect = new SetAngleEffect(color, nullptr, sDeg, degChanger, trickleWidth, nullptr, trickleWeight, nullptr, start, stop);
            effects.push_back(tEffect);
        }
    }

    Serial.print("   ++++++++ AC FREEHEAP:");
    Serial.print(ESP.getFreeHeap());
    Serial.print("\n");

    Serial.print("Trk Bup \n ");
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
