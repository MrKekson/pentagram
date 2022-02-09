
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

    Serial.print("Anim Death\n");
    Serial.print("------- BD FREEHEAP:");
    Serial.print(ESP.getFreeHeap());

    for (BaseEffect *e : effects)
    {
        delete e;
    }
    Serial.print(" ------ AD FREEHEAP:");
    Serial.print(ESP.getFreeHeap());
    Serial.print("\n");
}

void Animation::Setup(std::vector<BaseEffect *> P_effects)
{
    Serial.print("Anim Setup\n");

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
}

void Animation::Update(int64_t now)
{
    if ((_startTimestamp + MAX_ANIM_LENGHT) < now || _longestEffectTime < now) // anim végénél is true kell legyen
    {
        isFinished = true;
    }
    else
    {

        for (BaseEffect *e : effects)
        {
            if (e->isStarted(now) && !e->isEnded(now))
            {
                e->CalcStep(now);
            }
        }
    }
}
