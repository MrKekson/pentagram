#pragma once

#include <Arduino.h>
#include <vector.h>
#include <memory>

#include "effects.h"
#include "renderer.h"
#include "debug.h"
#include "base.h"
#include "animation.h"
#include "animations/rotate300.h"

class AnimationHandler
{
private:
    u_int _loopCount = 0;
    u_int _setupCount = 0;
    Renderer _rndr;

public:
    Animation *animationCurrent;
    AnimationHandler();
    ~AnimationHandler();
    void Setup();
    void Loop(int64_t now);

    void ProcessAnimation();
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
    Serial.print(":loooooooooooooooooooooooooooooooooop");
    _loopCount++;
    if (animationCurrent->isFinished)
    {
        delete animationCurrent;
        ProcessAnimation();
    }

    animationCurrent->Update(now);
    Serial.print("----------------------------------");
    _rndr.Render(animationCurrent->effects);
    Serial.print("********************\n");
}

void AnimationHandler::ProcessAnimation()
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

void AnimationHandler::Setup()
{
    _rndr.Setup();
    ProcessAnimation();
}
