
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
    ColorDelta baseColor;
    std::vector<ParsedEffectPartData> ratiosData;

    Animation(SData dta, ColorDelta baseC);
    ~Animation();

    std::vector<BaseEffect *> effects;

    int Start();
    void Setup(std::vector<BaseEffect *> effects, int64_t _startTimeStamp);
    void Update(int64_t now);
};

Animation::Animation(SData dta, ColorDelta baseC) : data(dta), baseColor(baseC)
{
}

Animation::~Animation()
{
    for (BaseEffect *e : effects)
    {
        delete e;
    };
}

void Animation::Setup(std::vector<BaseEffect *> P_effects, int64_t now)
{
    _startTimeStamp = now;
    effects = P_effects;
    for (BaseEffect *e : effects)
    {
        e->setAnimationStartTime(now);

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
    }
}
