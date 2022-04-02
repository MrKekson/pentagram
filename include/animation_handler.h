#pragma once

#include <Arduino.h>
#include <vector.h>
#include <memory>

#include "effects.h"
#include "renderer.h"
#include "debug.h"
#include "base.h"
#include "animation.h"
#include "animation_elements.h"

class AnimationHandler
{
private:
    u_int _loopCount = 0;
    u_int _setupCount = 0;
    Renderer *_rndr;

public:
    // Animation *animationCurrent;
    AnimationHandler();
    ~AnimationHandler();
    void Setup(Renderer *rndr, ParsedAnimationData *eData);
    void Loop();

    // DCHSV GetColourTransitionEndpoint(DCHSV colorTo, int64_t now, int64_t end);
    // void SetBaseColor(DCHSV c);

    Animation *CreateAnimation(SData prevData, ColorDelta delta, int64_t now);

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
    // Serial.print("WHAT THE");
    for (Animation *a : animations)
    {
        delete a;
        // Serial.print(" FUCK");
    }
}

void AnimationHandler::Loop()
{
    int64_t now = esp_timer_get_time();

    // Serial.print('NOW: ');
    // Serial.print(now);
    // Serial.print(' ');

    std::vector<BaseEffect *> runningEffects;
    runningEffects.insert(runningEffects.end(), baseEffects.begin(), baseEffects.end());

    for (int i = animations.size() - 1; i >= 0; i--)
    {
        Animation *a = animations[i];

        a->Update(now);
        if (a->isFinished)
        {
            // Serial.print("wtf:  ");
            // Serial.print(animations.size());
            // Serial.print(" smash ");
            // Serial.print(i);
            // Serial.print("; ");
            SData temp = a->data;
            ColorDelta tempBaseCol = a->baseColor;
            delete a;
            animations[i] = CreateAnimation(temp, tempBaseCol, now); // eraSE(BEGIN, REMOVE(BEGIN,END))v.erase(std::remove_if(v.begin(), v.end(), IsOdd), v.end());
        }

        runningEffects.insert(runningEffects.end(), animations[i]->effects.begin(), animations[i]->effects.end());
    }

    _rndr->Render(runningEffects, now);
}

SData CreateNextSData(SData prev, ColorDelta colorDelta) // make configurable and range based eg colour +- range etc
{
    SData symbolNew = prev;

    int rnd = esp_random() % 100;

    if (colorDelta.chance > 0)
    {
        int rndChn = esp_random() % 100;

        if (rndChn > colorDelta.chance)
        {

            double hStep = ((colorDelta.deltaH * 2) / 99 * rnd) - colorDelta.deltaH;

            Serial.print(hStep);
            Serial.print(" yolo ");

            double sStep = ((colorDelta.deltaS * 2) / 99 * rnd) - colorDelta.deltaH;
            double vStep = ((colorDelta.deltaV * 2) / 99 * rnd) - colorDelta.deltaH;

            symbolNew.c.H = Clamp(colorDelta.H + hStep, 255);

            symbolNew.c.S = Clamp(colorDelta.S + sStep, 255);
            symbolNew.c.V = Clamp(colorDelta.V + vStep, 255);

            Serial.print(symbolNew.c.H);
            Serial.print(" swag \n");
        }
    }

    symbolNew.symbol = esp_random() % 12;

    return symbolNew;
}

void CreateHolder(std::vector<BaseEffect *> &effects, SData prevData)
{
    int ePlay = esp_random() % 100; // shoulb be weighed chnage chooser, then weighted effect chances

    int64_t animEndTime = (rand() % 10 + 2) * SEC_TO_MICRO; // must be at least 2 or it can get jumpy

    SData newData = prevData;
    newData.weight = 30;

    switch (ePlay)
    {
    case 0 ... 49:
        effects = LightAndHold(prevData, prevData, 0, animEndTime);
        break;
    default:
        effects = LightAndHold(prevData, newData, 0, animEndTime);
        break;
    }
}

void CreateChanger(std::vector<BaseEffect *> &effects, SData prevData, SData nextData)
{
    int ePlay = esp_random() % 100 + 1; // shoulb be weighed chnage chooser, then weighted effect chances

    int64_t animEndTime = (rand() % 7 + 3) * SEC_TO_MICRO; // should be diff for chnage and hold
    // int64_t animEndTime = (3) * SEC_TO_MICRO; // should be diff for chnage and hold

    // achance BCHANCE
    switch (ePlay)
    {
        // 1..ACHANCE

    case 1 ... 20:
        effects = Trickle(prevData, nextData, 0, animEndTime);
        break;
        // ACHANCE .. ACHANCE+BCHANCE
    case 21 ... 50:
        effects = RotateTo(prevData, nextData, 0, animEndTime);
        break;
        // ACHANCE .. ACHANCE+BCHANCE
    default:
        effects = FadeTo(prevData, nextData, 0, animEndTime);
        break;
    }
    // Serial.print(" DONE");
}

Animation *AnimationHandler::CreateAnimation(SData prevData, ColorDelta baseColor, int64_t now)
{
    int ePlay = esp_random() % 10 + 1; // shoulb be weighed chnage chooser, then weighted effect chances

    std::vector<BaseEffect *> effects;

    SData newData;

    switch (ePlay)
    {
    case 1 ... 3:
        newData = CreateNextSData(prevData, baseColor);
        CreateChanger(effects, prevData, newData);
        break;

    default:
        newData = prevData;
        CreateHolder(effects, prevData);
        break;
    }

    Animation *animationCurrent = new Animation(newData, baseColor);

    animationCurrent->Setup(effects, now);
    return animationCurrent;
}

void AnimationHandler::Setup(Renderer *rndr, ParsedAnimationData *aData)
{
    int64_t now = esp_timer_get_time();

    _rndr = rndr;

    baseColor = DCHSV(aData->baseColor);
    ColorDelta deltas = ColorDelta(baseColor, 40, 20, 20, 100000);

    // CHSV effectColor = CHSV(180, 192, 192);
    DCHSV effectColor2 = DCHSV(140, 192, 192);
    // CHSV effectColor3 = CHSV(120, 192, 192);

    // ColorChanger colChanger = createLinearColorChanger(startData.c, endData.c, duration);

    BaseEffect *backgroundEffect = new BaseEffect(baseColor, nullptr, 0, nullptr, 360, nullptr, 48, nullptr, 0, 0);
    backgroundEffect->isFullRenderTime = true;
    baseEffects.push_back(backgroundEffect);

    for (ParsedEffectData e : aData->effectDatas)
    {
        int rnd = esp_random() % 12;
        Serial.print("ecreate:");
        Serial.print(e.effectColor.H);
        Serial.println(e.effectDelta.deltaH);
        animations.push_back(CreateAnimation(SData(e.effectColor, rnd, e.effectDelta.weight), e.effectDelta, now));
    }
    //_rndr.Setup();
    // animations.push_back(CreateAnimation(SData(effectColor2, 0, 350), deltas, now));
    // animations.push_back(CreateAnimation(SData(effectColor2, 4, 350), deltas, now));
    // animations.push_back(CreateAnimation(SData(effectColor2, 8, 350), deltas, now));
}
