#pragma once

#include <vector.h>

#include "debug.h"
#include "base.h"

#include "Effects/BaseEffect.h"
#include "Effects/UpdateSector.h"

// circle map to leds, mixing and displaying running effect arrays
class EffectHandler
{
private:
    std::vector<BaseEffect *> effects;
    CHSV _data[NUM_DEG];
    int _last_time;
    double r_weight = 1;
    Renderer& _renderer;

public:
    void AddEffect(BaseEffect *e)
    {
        effects.push_back(e);
    }

    void RemoveEffect(BaseEffect *e)
    {
        effects.erase(std::remove(effects.begin(), effects.end(), e), effects.end());
    }
    EffectHandler(Renderer& renderer): _renderer(renderer) {}
    ~EffectHandler()
    {
        for (auto e : effects)
        {
            delete e;
        }
    }

    void DoStuff() {
        for (auto e : effects)
        {
            e->DoStuff();
        }

        _renderer.Render(effects);
    }

    void Start()
    {
        this->AddEffect(new BaseEffect(CHSV(160, 96, 96), 1));
        this->AddEffect(new RotateSector(CHSV(120, 92, 192), 0, 10, 6, 1, 130));//Pipe(eFFECT ...)
        //effect().addrotate().addcolourchange();
        //this->AddEffect(new SetSymbol(CHSV(200, 128, 128), 0, 100));
        this->AddEffect(new SetSymbol(CHSV(120, 92, 192), 3, 100));
        this->AddEffect(new SetSymbol(CHSV(120, 92, 192), 7, 100));
        this->AddEffect(new SetSymbol(CHSV(120, 92, 192), 11, 100));
        
        //int64_t deltaTime;
        //int complitionRatio = 0 - 1;


        //this->AddEffect(new UpdateSector2(CHSV(40, 192, 192), [](double deg) { return 1.0; }, 10, 128));

        // this->AddEffect(new Effect(CHSV(160, 128, 128), 1));
        // this->AddEffect(new SymbolSimpleFade(CHSV(0, 192, 192), 9, 10000, 200));
        // this->AddEffect(new LightSection(70, 10, .5, false, CHSV(70, 192, 192), 130));
        // this->AddEffect(new Wobble(CRGB::AliceBlue, 2));
        
        // this->AddEffect(new DeepLightEffect(CHSV(180, 200, 192), 8, 8, -4, 256));
        // this->AddEffect(new LightSection(282, 45, 3.14, true, CHSV(180, 255, 198), 130));
        // this->AddEffect(new LightSection(40, 15, 1, false, CHSV(140, 200, 180), 130));



        //90 degree position tests
        // this->AddEffect(new UpdateSector(CHSV(0, 192, 192), 0, 10, 128));
        // this->AddEffect(new UpdateSector(CHSV(64, 192, 192), 90, 10, 128));
        // this->AddEffect(new UpdateSector(CHSV(128, 192, 192), 180, 10, 128));
        // this->AddEffect(new UpdateSector(CHSV(192, 192, 192), 270, 10, 128));

        

        _last_time = millis();
    }

    // double(double) hFvg(int &completionRatio, degCalcFuntionType &calc))) {
        
    // }
};
