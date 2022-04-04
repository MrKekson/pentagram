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
    void Reset();

    Animation *CreateAnimation(SData prevData, ColorDelta delta, std::vector<ParsedEffectPartData> ratios, int64_t now);

    CHSV baseColor;
    std::vector<SData> symbolData;

    std::vector<BaseEffect *> baseEffects;
    std::vector<Animation *> animations;
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

void AnimationHandler::Reset()
{
    baseEffects.clear();
    animations.clear();
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
            SData temp = a->data;
            ColorDelta tempBaseCol = a->baseColor;
            std::vector<ParsedEffectPartData> ratios = a->ratiosData;
            delete a;
            animations[i] = CreateAnimation(temp, tempBaseCol, ratios, now);
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

        if (rndChn < colorDelta.chance)
        {

            double hStep = ((colorDelta.deltaH * 2) / 99 * rnd) - colorDelta.deltaH;
            symbolNew.c.H = Clamp(colorDelta.H + hStep, 255);

            Serial.print(hStep);
            Serial.print(" yolo ");

            // double sStep = ((colorDelta.deltaS * 2) / 99 * rnd) - colorDelta.deltaS;
            // double vStep = ((colorDelta.deltaV * 2) / 99 * rnd) - colorDelta.deltaV;

            // symbolNew.c.S = Clamp(colorDelta.S + sStep, 255);
            // symbolNew.c.V = Clamp(colorDelta.V + vStep, 255);

            Serial.print(symbolNew.c.H);
            Serial.print(" swag \n");
        }
    }

    symbolNew.symbol = esp_random() % 12;

    return symbolNew;
}

SData CreateEffcetsById(std::vector<BaseEffect *> &effects, int effectId, SData prevData, SData nextData, int64_t animEndTime)
{
    SData retData = nextData;
    switch (effectId)
    {
    case 1:
        effects = Trickle(prevData, nextData, 0, animEndTime);
        break;
    case 2:
        effects = RotateTo(prevData, nextData, 0, animEndTime);
        break;
    case 3:
        effects = FadeTo(prevData, nextData, 0, animEndTime);
        break;
    case 1000:
        effects = LightAndHold(prevData, prevData, 0, animEndTime);
        retData = prevData;
        break;
    case 1001:
        effects = LightAndHold(prevData, nextData, 0, animEndTime);
        retData = prevData;
        break;
    }
    return retData;
}

Animation *AnimationHandler::CreateAnimation(SData prevData, ColorDelta colorDelta, std::vector<ParsedEffectPartData> ratios, int64_t now)
{
    std::vector<BaseEffect *> effects;
    SData newData;

    static int effectNum = ratios.size();

    int effectRatioEndpoints[effectNum + 1];

    Serial.print(ratios.size());
    Serial.print("- ratiosize \n");

    int ratioCount = 0;

    for (int i = 0; i < effectNum; i++)
    {
        if (colorDelta.weight > 0)
        {
            Serial.print(ratioCount);
            Serial.print(" :rcount ");
            Serial.print(ratios[i].ratio);
            Serial.print(" :ratio  \n");
        }
        effectRatioEndpoints[i] = ratioCount + ratios[i].ratio;
        ratioCount += ratios[i].ratio;
    }

    int chn = esp_random() % ratioCount;

    if (colorDelta.weight > 0)
    {
        Serial.print(chn);
        Serial.print(" :chn  \n");
    }
    bool finished = false;
    int i = 0;

    while (i < effectNum && !finished)
    {
        if (chn < effectRatioEndpoints[i])
        {
            if (colorDelta.weight > 0)
            {
                Serial.print(effectRatioEndpoints[i]);
                Serial.print(" :end  ");
                Serial.print(chn);
                Serial.print(" :chn  \n");
            }
            newData = CreateNextSData(prevData, colorDelta);
            int64_t endTime = (rand() % ratios[i].maxTime + ratios[i].minTime) * 1000;

            newData = CreateEffcetsById(effects, ratios[i].typeId, prevData, newData, endTime);
            finished = true;
        }
        i++;
    }

    Animation *animationCurrent = new Animation(newData, colorDelta);
    animationCurrent->ratiosData = ratios;

    animationCurrent->Setup(effects, now);
    return animationCurrent;
}

void AnimationHandler::Setup(Renderer *rndr, ParsedAnimationData *aData)
{
    int64_t now = esp_timer_get_time();
    _rndr = rndr;

    BaseEffect *backgroundEffect = new BaseEffect(aData->baseColor, nullptr, 0, nullptr, 360, nullptr, 50, nullptr, 0, 0);
    backgroundEffect->isFullRenderTime = true;
    baseEffects.push_back(backgroundEffect);

    for (ParsedEffectData e : aData->effectDatas)
    {
        if (e.effectDelta.weight > 0)
        {
            int rnd = esp_random() % 12;
            animations.push_back(CreateAnimation(SData(e.effectColor, rnd, e.effectDelta.weight), e.effectDelta, e.partDatas, now));
        }
    }
}
