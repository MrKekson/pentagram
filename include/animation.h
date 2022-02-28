
#pragma once

#include <Arduino.h>
#include <vector.h>

#include "effects.h"
#include "renderer.h"

class Animation
{
private:
    int64_t _elapsedTime = 0;
    int64_t _startTimeStamp;
    int64_t _longestEffectTime = 0;

public:
    bool isFinished = false;

    SData data;

    Animation(SData dta);
    ~Animation();

    std::vector<BaseEffect *> effects;

    int Start();
    void Setup(std::vector<BaseEffect *> effects);
    void Update();
};

Animation::Animation(SData dta) : data(dta)
{
}

Animation::~Animation()
{

    // Serial.print("Anim Death\n");
    // Serial.print("------- BD FREEHEAP:");
    // Serial.print(ESP.getFreeHeap());

    for (BaseEffect *e : effects)
    {
        // Serial.print('e');
        delete e;
    }

    // Serial.print("\n ------ AD FREEHEAP:");
    // Serial.print(ESP.getFreeHeap());
    // Serial.print("\n");
}

void Animation::Setup(std::vector<BaseEffect *> P_effects)
{
    // Serial.print("Anim Setup\n");

    _startTimeStamp = esp_timer_get_time();
    effects = P_effects;
    for (BaseEffect *e : effects)
    {
        e->setAnimationStartTime(_startTimeStamp);

        if (e->endTime > 0 && e->endTime > _longestEffectTime)
        {
            _longestEffectTime = e->endTime;
        }
    }
}

void Animation::Update()
{
    int64_t now = esp_timer_get_time();
    if ((_startTimeStamp + MAX_ANIM_LENGHT) < now || _longestEffectTime <= now) // anim végénél is true kell legyen
    {
        isFinished = true;
        return;
    }

    for (BaseEffect *e : effects)
    {
        // e->isRunning = false;
        e->CalcStep();
        // e->isRunning = true;

        // Serial.print(e->isStarted(now));
        // Serial.print(!e->isEnded(now));
        // Serial.print(e->isRunning);
        // Serial.print("\n");
    }
}
