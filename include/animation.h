
#pragma once

#include <Arduino.h>
#include <vector.h>

#include "effects.h"
#include "renderer.h"

class Animation
{
private:
    int64_t _elapsedTime = 0;
    int64_t _startTimestamp;

public:
    Animation();
    ~Animation();

    std::vector<BaseEffect *> effects;

    virtual int Start();
    void Setup();
    void Update();
};

Animation::Animation()
{
}

Animation::~Animation()
{
    for (auto e : effects)
    {
        delete e;
    }
}

void Animation::Setup()
{
    auto color = CHSV(190, 150, 150);
    auto deg = 90;
    auto width = 50; // degree
    int strength = 128;

    auto baseColor = CHSV(160, 96, 96);
    auto baseColorEffect = new SetAngleEffect(
        baseColor, 0, [](double D, int64_t t)
        { return 0.0; },
        360, 1);
    effects.push_back(baseColorEffect);

    // auto effect = new SetAngleEffect(
    //     color, deg, [](double D)
    //     { return Clamp(D + 1); },
    //     width, 255);
    // effect->startTime = _startTimestamp + 5 * SEC_TO_MICRO;
    // effect->endTime = _startTimestamp + 25 * SEC_TO_MICRO;
    // effects.push_back(effect);

    // auto effect2 = new SetAngleEffect(
    //     color, Clamp(deg + 180.0), [](double D)
    //     { return Clamp(D + 1); },
    //     width, 255);

    // effect2->startTime = _startTimestamp + 5 * SEC_TO_MICRO;
    // effect2->endTime = _startTimestamp + 25 * SEC_TO_MICRO;
    // effects.push_back(effect2);

    // auto effect3 = new SetAngleEffect(
    //     color, 120, [](double D)
    //     { return 120.0; },
    //     width, 255);

    // effects.push_back(effect3);

    auto startTime = _startTimestamp + 3 * SEC_TO_MICRO;
    auto endTime = _startTimestamp + 18 * SEC_TO_MICRO;
    auto lambda = [startTime, endTime](double D, int64_t t)
    {
        // auto startTime = 3 * SEC_TO_MICRO;
        // auto endTime = 8 * SEC_TO_MICRO;

        auto duration = endTime - startTime;
        auto deltaT = t - startTime;
        auto progress = (double)deltaT / (double)duration;

        auto step = 300.0 * (double)progress;
        // if (D > 60 && D < 360)
        // {
        return Clamp(step + 60);
        //}
        return D;
    };

    auto effect = new SetAngleEffect(
        color,
        60,
        lambda,
        width,
        255);

    effect->startTime = startTime;
    effect->endTime = endTime;
    effects.push_back(effect);
    // auto calculatedDeg = new (startingDegree, endingDegree);
    // A => B
    // Linear
    // 0..1

    // effect->setDeg();
}

int Animation::Start()
{
    _startTimestamp = esp_timer_get_time();

    // this->AddEffect(new BaseEffect(CHSV(160, 96, 96), 0, 180, 10));

    // this->AddEffect(new SetSymbolEffect(CHSV(120, 92, 192), 3, 100));
    // this->AddEffect(new SetSymbolEffect(CHSV(120, 92, 192), 7, 100));
    // this->AddEffect(new SetSymbolEffect(CHSV(120, 92, 192), 11, 100));
}

void Animation::Update()
{
    // _elapsedTime = esp_timer_get_time() - _startTimestamp;
    auto now = esp_timer_get_time();
    // auto newDeg = Calcdeg(strategyDouble strat){}
    for (auto e : effects)
    {
        if (e->startTime < now && e->endTime > now)
        {
            e->CalcStep(now);
        }
    }
    //  update effect parameters here
}
