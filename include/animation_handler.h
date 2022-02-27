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

    CHSV baseColor;
    std::vector<SData> symbolData;
    std::vector<std::pair<SData, SData>> CreateRandomSymbolPairs();
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
    // Serial.print(":loooooooooooooooooooooooooooooooooop");
    //_loopCount++;
    animationCurrent->Update(now);
    if (!animationCurrent->isFinished)
    {
        _rndr.Render(animationCurrent->effects);
    }

    if (animationCurrent->isFinished)
    {
        delete animationCurrent;
        ProcessAnimation();
    }

    // Serial.print("----------------------------------");

    // Serial.print("********************\n");
}

std::vector<std::pair<SData, SData>> AnimationHandler::CreateRandomSymbolPairs() // make configurable
{
    std::vector<std::pair<SData, SData>> retVect;
    for (SData &symbol : symbolData)
    {
        SData symbol2 = symbol;

        symbol2.symbol = rand() % 12;

        retVect.push_back(std::make_pair(symbol, symbol2));
        symbol = symbol2;
    }
    return retVect;
}

void AnimationHandler::ProcessAnimation()
{
    int ePlay = rand() % 1; // shoulb be weighed chnage chooser, then weighted effect chances

    int animEndTime = (3 + rand() % 7) * SEC_TO_MICRO; // should be diff for chnage and hold

    animationCurrent = new Animation();
    std::vector<BaseEffect *> effects;

    int rnd = rand() % 12;

    switch (ePlay)
    {
    case 0:
        effects = LightAndHold(symbolData, 0, animEndTime);

        break;
    case 1:
        effects = Trickle(CreateRandomSymbolPairs(), 0, animEndTime);
        break;
    default:
        break;
    }

    // Add background effect once here
    BaseEffect *backgroundEffect = new BaseEffect(baseColor, nullptr, 0, nullptr, 360, nullptr, 8, nullptr, 0, animEndTime);
    effects.push_back(backgroundEffect);

    animationCurrent->Setup(effects);

    // Serial.print(_loopCount);
    // Serial.print(":loop Creating Anim --- ");
    // animationCurrent = new Animation();

    // Serial.print("Creating Effect ----");

    // _setupCount++;
    // Serial.print("Setup Count ----");
    // Serial.print(_setupCount);
    // Serial.print("\n");

    // std::vector<BaseEffect *> effects = Trickle(vect symol);
    // Serial.print(effects.size());
    // Serial.print("Adding Effect ");
    // size_t bytes = sizeof(BaseEffect) * effects.size();
    // Serial.print(" B ESize: ");
    // Serial.print(bytes);

    // animationCurrent->Setup(effects);
    // Serial.print("   DONE \n");
}

void AnimationHandler::Setup()
{
    baseColor = CHSV(160, 92, 64);

    symbolData.push_back(SData(CHSV(180, 128, 128), 0, 192));
    symbolData.push_back(SData(CHSV(120, 128, 128), 11, 192));
    symbolData.push_back(SData(CHSV(0, 128, 128), 9, 192));

    _rndr.Setup();
    ProcessAnimation();
}
