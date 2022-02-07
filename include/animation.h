
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
    int64_t _longestEffectTime = 0;

public:
    bool isFinished = false;

    Animation();
    ~Animation();

    std::vector<BaseEffect *> effects;

    int Start();
    void Setup(std::vector<BaseEffect *> effects);
    void Update(int64_t now);
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

void Animation::Setup(std::vector<BaseEffect *> P_effects)
{
    _startTimestamp = esp_timer_get_time();
    effects = P_effects;
    for (auto e : effects)
    {
        e->setAnimationStartTime(_startTimestamp);

        if (e->endTime > 0 && e->endTime > _longestEffectTime)
        {
            _longestEffectTime = e->endTime;
        }
    }

    // Serial.write("" + effect->startTime);
    // Serial.write("-----");
    // Serial.write("" + effect->endTime);
    // Serial.write("\n");
    // auto calculatedDeg = new (startingDegree, endingDegree);

    // effect->setDeg();
}

int Animation::Start()
{
}

void Animation::Update(int64_t now)
{

    if ((_startTimestamp + MAX_ANIM_LENGHT) < now || _longestEffectTime < now) // anim végénél is true kell legyen
    {
        isFinished = true;
    }

    // _elapsedTime = esp_timer_get_time() - _startTimestamp;

    // auto newDeg = Calcdeg(strategyDouble strat){}

    for (auto e : effects)
    {
        if (e->isStarted(now) && !e->isEnded(now))
        {

            // Serial.write("-----");
            // Serial.write("\n");
            e->CalcStep(now);
        }
    }
    //  update effect parameters here
}
