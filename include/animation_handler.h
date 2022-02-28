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
    // Animation *animationCurrent;
    AnimationHandler();
    ~AnimationHandler();
    void Setup();
    void Loop();

    void CreateAnimation(SData prevData);

    CHSV baseColor, effectColor;
    std::vector<SData> symbolData;

    std::vector<BaseEffect *> baseEffects;
    std::vector<Animation *> animations;
    // std::pair<SData, SData> CreateNextSData(SData &prev);
};

AnimationHandler::AnimationHandler()
{
}

AnimationHandler::~AnimationHandler()
{
    for (Animation *a : animations)
    {
        delete a;
    }
}

void AnimationHandler::Loop()
{
    int64_t now = esp_timer_get_time();
    // Serial.print(":loooooooooooooooooooooooooooooooooop");
    //_loopCount++;
    bool isAnyRendered = false;

    std::vector<BaseEffect *> runningEffects;
    runningEffects.insert(runningEffects.end(), baseEffects.begin(), baseEffects.end());

    // for (Animation *a : animations)
    for (int i = animations.size() - 1; i >= 0; i--)
    {
        Animation *a = animations[i];

        a->Update();

        runningEffects.insert(runningEffects.end(), a->effects.begin(), a->effects.end());

        if (a->isFinished)
        {
            Serial.print("smash\n");
            SData temp = a->data;
            delete a;
            animations.erase(animations.begin() + i);
            CreateAnimation(temp);
        }
        else
        {
        }
    }

    _rndr.Render(runningEffects);

    // Serial.print("----------------------------------");

    // Serial.print("********************\n");
}

SData CreateNextSData(SData prev) // make configurable and range based eg colour +- range etc
{
    SData symbolNew = prev;

    symbolNew.symbol = rand() % 12;

    return symbolNew;
}

void AnimationHandler::CreateAnimation(SData prevData)
{
    int ePlay = rand() % 5; // shoulb be weighed chnage chooser, then weighted effect chances

    int animEndTime = (3 + rand() % 2) * SEC_TO_MICRO; // should be diff for chnage and hold

    std::vector<BaseEffect *> effects;

    SData newData;
    Serial.print("sdata\n");
    // for (SData s : symbolData)
    {
        switch (ePlay)
        {
        case 0 ... 2:
            newData = prevData;
            effects = LightAndHold(prevData, prevData, 0, animEndTime);

            break;
        case 3 ... 4:
            newData = CreateNextSData(prevData);
            effects = Trickle(prevData, newData, 0, animEndTime);
            break;
        default:
            break;
        }
    }

    // Add background effect once here

    Serial.print("newanim\n");

    Animation *animationCurrent = new Animation(newData);

    animationCurrent->Setup(effects);
    animations.push_back(animationCurrent);
}

void AnimationHandler::Setup()
{
    baseColor = CHSV(160, 64, 80);
    effectColor = CHSV(180, 192, 192);

    BaseEffect *backgroundEffect = new BaseEffect(baseColor, nullptr, 0, nullptr, 360, nullptr, 48, nullptr, 0, 0);
    backgroundEffect->isFullRenderTime = true;
    baseEffects.push_back(backgroundEffect);

    // symbolData.push_back(SData(effectColor, 0, 350));
    //  symbolData.push_back(SData(effectColor, 11, 300));
    //   symbolData.push_back(SData(CHSV(0, 128, 128), 9, 192));

    _rndr.Setup();
    CreateAnimation(SData(effectColor, 0, 350));
    CreateAnimation(SData(effectColor, 6, 350));
}
