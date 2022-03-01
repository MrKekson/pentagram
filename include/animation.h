
#pragma once

#include <Arduino.h>
#include <vector.h>

#include "effects.h"
#include "renderer.h"

class Animation
{
private:
    bool _isLastFrame = false;
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
    void Setup(std::vector<BaseEffect *> effects, int64_t _startTimeStamp);
    void Update(int64_t now);

    bool isLastFrame()
    {
        if (_isLastFrame == true)
        {
            _isLastFrame = false;
            return true;
        }
        return false;
    }
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

void Animation::Setup(std::vector<BaseEffect *> P_effects, int64_t _startTimeStamp)
{
    // Serial.print("Anim Setup\n");

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

void Animation::Update(int64_t now)
{
    if ((_startTimeStamp + MAX_ANIM_LENGHT) < now || _longestEffectTime <= now) // anim végénél is true kell legyen
    {
        isFinished = true;
        return;
    }

    for (BaseEffect *e : effects)
    {

        e->CalcStep(now);

        // e->isRunning = false;

        // e->isRunning = true;

        // Serial.print(e->isStarted(now));
        // Serial.print(!e->isEnded(now));
        // Serial.print(e->isRunning);
        // Serial.print("\n");
    }
}
