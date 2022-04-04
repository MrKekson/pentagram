#include <Arduino.h>
#include <FastLED.h>

#include "json_handler.h"
#include "renderer.h"
#include "animation_handler.h"

#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000 / x)

long framecount = 0;
int renderTime, loopTime, loopCount, totalLoops = 0;
uint32_t meminfo;

// EffectHandler eHandler = EffectHandler(renderer);

AnimationHandler animHandler = AnimationHandler();
Renderer renderer = Renderer();
JsonHandler jsonHandler = JsonHandler();

#include "wifi_handler.h"

void setup()
{
  Serial.begin(115200);

  renderer.Setup();

  Serial.println("renderer..OK");
  renderer.SetColor(CHSV(0, 92, 92));

  ParsedAnimationData animations = jsonHandler.Read();

  Serial.println("config..OK");
  WifiSetup();
  Serial.println("wifi..OK");

  renderer.SetBrightness(animations.brightnes);
  animHandler.Setup(&renderer, &animations);
}

void loop()
{
  EVERY_N_MILLISECONDS(100)
  {
    server.handleClient();
  }

  EVERY_N_MILLISECONDS(1000 / FPS)
  {
    loopCount++;
    // renderer.Render();
    int64_t now = esp_timer_get_time();
    // eHandler.Render(now);
    int rTime = esp_timer_get_time() - now;
    renderTime = (renderTime + rTime + rTime) / 3;

    animHandler.Loop();
    int lTime = esp_timer_get_time() - now;
    loopTime = (loopTime + lTime + lTime) / 3;
  }

  // EVERY_N_MILLISECONDS(1000)
  // {
  //   Serial.print("\n");
  //   Serial.print(loopCount);
  //   Serial.print("FPS -- ");
  //   Serial.print(renderTime / 1000);
  //   Serial.print(" R ");
  //   Serial.print(totalLoops);
  //   Serial.print(" :TOTAL ");
  //   uint32_t memDelta = (meminfo - ESP.getFreeHeap()) / 1024;
  //   Serial.print(memDelta);
  //   Serial.print(" M ");

  //   meminfo = ESP.getFreeHeap();
  //   Serial.print(meminfo);
  //   Serial.print("\n");
  //   loopCount = 0;
  //   totalLoops++;
  //   renderTime = 0;
  //   loopTime = 0;
  // }
}
