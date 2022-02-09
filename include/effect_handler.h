#pragma once

#include <vector.h>

#include "debug.h"
#include "base.h"

#include "effects.h"

// circle map to leds, mixing and displaying running effect arrays
class EffectHandler
{
private:
    // CHSV _data[NUM_DEG];
    // int _last_time;
    double r_weight = 1;
    Renderer &_renderer;

public:
    std::vector<BaseEffect *> *effects = nullptr;

    EffectHandler(Renderer &renderer) : _renderer(renderer) {}
    ~EffectHandler()
    {
        // for (auto e : effects)
        // {
        //     delete e;
        // }
    }

    void Render(int64_t now)
    {
        bool changed = false;
        if (effects == nullptr)
        {
            // Serial.write("SUPA MEGA ATOM ERROR");
        }
        for (auto e : (*effects))
        {
            e->Render(now);
            changed = true;
        }
        // cdata vector?
        if (changed)
        {
            _renderer.Render(*effects);
        }
    }
};
