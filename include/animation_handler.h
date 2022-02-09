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
    u_int _loopCount = 0;
    u_int _setupCount = 0;

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
    _loopCount++;
    if (animationCurrent->isFinished)
    {
        delete animationCurrent;
        Setup();
    }

    animationCurrent->Update(now);
}

void AnimationHandler::Setup()
{
    Serial.print(_loopCount);
    Serial.print(":loop Creating Anim --- ");
    animationCurrent = new Animation();

    Serial.print("Creating Effect ----");

    _setupCount++;
    Serial.print("Setup Count ----");
    Serial.print(_setupCount);
    Serial.print("\n");

    std::vector<BaseEffect *> effects = Trickle();
    Serial.print(effects.size());
    Serial.print("Adding Effect ");
    size_t bytes = sizeof(effects[0]) * effects.size();
    Serial.print(" ESize: ");
    Serial.print(bytes);

    animationCurrent->Setup(effects);
    Serial.print("DONE \n");
}
