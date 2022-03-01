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

    Animation *CreateAnimation(SData prevData, int64_t now);

    CHSV baseColor;
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
    Serial.print("WHAT THE");
    for (Animation *a : animations)
    {
        delete a;
        Serial.print(" FUCK");
    }
}

void AnimationHandler::Loop()
{
    int64_t now = esp_timer_get_time();

    std::vector<BaseEffect *> runningEffects;
    runningEffects.insert(runningEffects.end(), baseEffects.begin(), baseEffects.end());

    for (int i = animations.size() - 1; i >= 0; i--)
    {
        Animation *a = animations[i];

        a->Update(now);
        if (a->isFinished)
        {
            Serial.print("\nsmash ");
            SData temp = a->data;
            delete a;
            animations[i] = CreateAnimation(temp, now); // eraSE(BEGIN, REMOVE(BEGIN,END))v.erase(std::remove_if(v.begin(), v.end(), IsOdd), v.end());
        }

        runningEffects.insert(runningEffects.end(), animations[i]->effects.begin(), animations[i]->effects.end());
    }

    _rndr.Render(runningEffects, now);
}

SData CreateNextSData(SData prev) // make configurable and range based eg colour +- range etc
{
    SData symbolNew = prev;

    // Serial.print(prev.symbol);
    // Serial.print(" :sym ");

    symbolNew.symbol = rand() % 12;

    return symbolNew;
}

void CreateHolder(std::vector<BaseEffect *> &effects, SData prevData)
{
    int ePlay = rand() % 100; // shoulb be weighed chnage chooser, then weighted effect chances

    int64_t animEndTime = (2 + rand() % 5) * SEC_TO_MICRO; // must be at least 2 or it can get jumpy

    SData newData = prevData;
    newData.weight = 30;

    switch (ePlay)
    {
    case 0 ... 49:
        effects = LightAndHold(prevData, prevData, 0, animEndTime, 1);
        break;
    default:
        effects = LightAndHold(prevData, newData, 0, animEndTime);
        break;
    }
}

void CreateChanger(std::vector<BaseEffect *> &effects, SData prevData, SData nextData)
{
    int ePlay = rand() % 100; // shoulb be weighed chnage chooser, then weighted effect chances

    int64_t animEndTime = (3 + rand() % 7) * SEC_TO_MICRO; // should be diff for chnage and hold

    switch (ePlay)
    {
    case 0 ... 19:
        effects = Trickle(prevData, nextData, 0, animEndTime);
        break;
    case 20 ... 49:
        effects = RotateTo(prevData, nextData, 0, animEndTime);
        break;

    default:
        effects = FadeTo(prevData, nextData, 0, animEndTime);
        break;
    }
}

Animation *AnimationHandler::CreateAnimation(SData prevData, int64_t now)
{
    int ePlay = rand() % 10; // shoulb be weighed chnage chooser, then weighted effect chances

    std::vector<BaseEffect *> effects;

    SData newData;
    Serial.print("create_sdata ");
    // for (SData s : symbolData)
    {
        switch (ePlay)
        {
        case 0 ... 1:
            newData = CreateNextSData(prevData);
            CreateChanger(effects, prevData, newData);
            break;

        default:
            newData = prevData;
            CreateHolder(effects, prevData);
            break;
        }
    }

    Serial.print("newanim\n");

    Animation *animationCurrent = new Animation(newData);

    animationCurrent->Setup(effects, now);
    return animationCurrent;
    // animationCurrent->Update(now);
    // animations.push_back(animationCurrent);
}

void AnimationHandler::Setup()
{
    int64_t now = esp_timer_get_time();
    baseColor = CHSV(180, 64, 80);
    CHSV effectColor = CHSV(180, 192, 192);
    CHSV effectColor2 = CHSV(140, 192, 192);
    CHSV effectColor3 = CHSV(120, 192, 192);

    BaseEffect *backgroundEffect = new BaseEffect(baseColor, nullptr, 0, nullptr, 360, nullptr, 48, nullptr, 0, 0);
    backgroundEffect->isFullRenderTime = true;
    baseEffects.push_back(backgroundEffect);

    _rndr.Setup();
    animations.push_back(CreateAnimation(SData(effectColor2, 0, 350), now));
    animations.push_back(CreateAnimation(SData(effectColor2, 4, 350), now));
    animations.push_back(CreateAnimation(SData(effectColor2, 8, 350), now));
}
