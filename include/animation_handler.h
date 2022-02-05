#pragma once
#include <vector.h>

#include "effects.h"
#include "effect_handler.h"
#include "debug.h"
#include "base.h"
#include "animation.h"

class AnimationHandler
{
private:
    Renderer &_renderer;

public:
    AnimationHandler(Renderer &renderer);
    ~AnimationHandler();
    void Startup();
};

AnimationHandler::AnimationHandler(Renderer &renderer) : _renderer(renderer)
{
}

AnimationHandler::~AnimationHandler()
{
}

void AnimationHandler::Startup()
{
    auto newAnimation = new Animation();
    newAnimation->Setup();
    newAnimation->Start();
}
