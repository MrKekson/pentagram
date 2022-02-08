#pragma once

#include <Arduino.h>
#include <vector.h>
#include <memory>

#include "effects.h"
#include "effect_handler.h"
#include "debug.h"
#include "base.h"
#include "animation.h"
#include "animations/rotate300.h"

class AnimationHandler
{
private:
public:
    Animation *animationCurrent;
    AnimationHandler();
    ~AnimationHandler();
    void Setup();
    void Loop(int64_t now);
};

AnimationHandler::AnimationHandler()
{
}

AnimationHandler::~AnimationHandler()
{
    delete animationCurrent;
}

void AnimationHandler::Loop(int64_t now)
{
    if (animationCurrent->isFinished)
    {
        delete animationCurrent;
        Setup();
    }

    animationCurrent->Update(now);
}

void AnimationHandler::Setup()
{
    animationCurrent = new JustBase();
    // auto effects = createRotate300();
    auto effects = Trickle();
    animationCurrent->Setup(effects);
}
