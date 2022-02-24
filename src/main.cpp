#include <Arduino.h>
#include <FastLED.h>
#include <ArduinoJson.h>

#include "SPIFFS.h"

// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiAP.h>

#include "renderer.h"
#include "effect_handler.h"
#include "animation_handler.h"

#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000 / x)
//#define ARRAYLENGTH(x) (sizeof(x) / sizeof(x[0]))

// CHSV hsv_leds[NUM__LEDS];
// CRGB leds[NUM__LEDS];

long framecount = 0;
int renderTime, loopTime, loopCount;
uint32_t meminfo;

void AdditionalCode();

// EffectHandler eHandler = EffectHandler(renderer);

AnimationHandler animHandler = AnimationHandler();

void setup()
{
  Serial.begin(115200);

  // AdditionalCode();

  animHandler.Setup();
  //  testAnimation.Setup();
  //  eHandler.effects = &(animHandler.animationCurrent->effects);
}

void loop()
{
  EVERY_N_MILLISECONDS(20)
  {
    loopCount++;
    // renderer.Render();
    int64_t now = esp_timer_get_time();
    // eHandler.Render(now);
    int rTime = esp_timer_get_time() - now;
    renderTime = (renderTime + rTime + rTime) / 3;

    // testAnimation.Update();
    animHandler.Loop(now);
    int lTime = esp_timer_get_time() - now;
    loopTime = (loopTime + lTime + lTime) / 3;
    // eHandler.effects = &(animHandler.animationCurrent->effects);
  }

  // EVERY_N_MILLISECONDS(1000)
  // {
  //   Serial.print(loopCount);
  //   Serial.print("FPS -- ");
  //   Serial.print(renderTime / 1000);
  //   Serial.print("Rt -- ");
  //   Serial.print(loopTime / 1000);
  //   Serial.print("Lt  ");
  //   auto memDelta = ESP.getFreeHeap() - meminfo;
  //   Serial.print(memDelta);
  //   Serial.print(" M ");

  //   meminfo = ESP.getFreeHeap();
  //   Serial.print(meminfo);
  //   Serial.print("\n");
  //   loopCount = 0;
  //   renderTime = 0;
  //   loopTime = 0;
  // }
}

void AdditionalCode()
{
  auto t = esp_timer_get_time;

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open("/test.json");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  auto fSize = file.size();

  char jsonFileData[fSize] = {'\0'};

  u_int i = 0;

  while (file.available())
  {
    jsonFileData[i] = file.read();
    Serial.write(jsonFileData[i]);
    i++;
  }
  jsonFileData[i] = '\0';

  file.close();

  u_int jsonSize = fSize * 1.04; // ke?
  DynamicJsonDocument doc(jsonSize);

  DeserializationError error = deserializeJson(doc, jsonFileData);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // const char* name = doc["name"];
  // Serial.println("name:");
  // Serial.println(name);
}

// void set(CRGB c)
// {
//   for (int i = 0; i < NUM__LEDS; ++i)
//   {
//     leds[i] = c;
//   }
// }

// void copyToCRGB()
// {
//   for (int i = 0; i < NUM_LEDS; i++)
//   {
//     CHSV color(171, 255, 255);
//     auto v = CHSV(32,32,42);
//     CRGB c2 = CRGB(CHSV(32, 32, 128));
//     leds[i].setHSV(color.h, color.s, color.v); //CRGB(64, 64, 64);  //CRGB(*hsv_leds[i]);
//   }
// }

// void reset(CRGB color)
// {
//   fill_solid(leds, NUM__LEDS, color);
// }

// void setup()
// {
//   //Serial.begin(9600);
//   // put your setup code here, to run once:
//   FastLED.addLeds<NEOPIXEL, 27>(leds, NUM__LEDS);

//   FastLED.setBrightness(brightness);
//   FastLED.clear();

//   fill_solid(leds, NUM__LEDS, CRGB::DeepSkyBlue);
//   //reset(CRGB(64, 64, 64));
//   //copyToCRGB();
// }

// #define SEGMENT_SIZE 1

// int maxNumberOfLoops = NUM__LEDS;
// int loopCounter = 0;

// void waveEffect()
// {
//   // EVERY_N_MILLISECONDS(200)
//   {
//     CRGB c = CRGB(32, 32, 128);
//     leds[loopCounter] = c;
//     leds[loopCounter + SEGMENT_SIZE] = c;
//     leds[loopCounter + SEGMENT_SIZE * 2] = c;
//     leds[loopCounter + SEGMENT_SIZE * 3] = c;

//     if (loopCounter == maxNumberOfLoops - 1)
//     {
//       loopCounter = 0;
//       reset(CRGB(0, 0, 0));
//       return;
//     }
//     //Serial.println("L: loopCounter");
//     ++loopCounter;
//   }
// }

// void loop()
// {
//   put your main code here, to run repeatedly:

//   waveEffect();
//   EVERY_N_MILLISECONDS(300)
//   {
//     for (int i = 0; i < NUM__LEDS; i++)
//     {
//       leds[i] = CRGB::Red;
//     }
//   }
//   FastLED.setBrightness(brightness);
//   FastLED.show();
//   Render();
//   delay(200);
// }

// #define WAVE_SIZE 3

// CHSV hsv_leds[NUM__LEDS + WAVE_SIZE*2];

// CRGB ex_leds[NUM__EX_LEDS];
// CHSV hsv_exLeds[NUM__EX_LEDS];

// CHSV colors[NUM__COLORS];

// int currentColor = 3;
// int g_brightness = 159;
// int minBrightness = 31;
// int maxBrightness = 255;
// int step = 32;

// bool running = false;

// const char *ssid = "dva";
// // const char *password = "1234";

// WiFiServer server(80);

// void createColors() {
//     colors[0] = CHSV(0, 255, 100);
//     colors[1] = CHSV(32, 255, 100);
//     colors[2] = CHSV(64, 255, 100);
//     colors[3] = CHSV(96, 255, 100);
//     colors[4] = CHSV(128, 255, 100);
//     colors[5] = CHSV(160, 255, 100);
//     colors[6] = CHSV(192, 255, 100);
//     colors[7] = CHSV(224, 255, 100);
// }

// void reset(CHSV color){
//     for(int i = 0; i < NUM__LEDS + WAVE_SIZE * 2; ++i)
//     {
//         hsv_leds[i] = color;
//     }

//     for(int i = 0; i < NUM__EX_LEDS; ++i)
//     {
//         hsv_exLeds[i] = color;
//     }
// }

// void setup() {
//     FastLED.addLeds<NEOPIXEL, 27>(leds, NUM__LEDS);
//     FastLED.addLeds<NEOPIXEL, 25>(ex_leds, NUM__EX_LEDS);

//     FastLED.setBrightness(g_brightness);
//     FastLED.clear();

//     createColors();
//     reset(colors[currentColor]);

//     WiFi.softAP(ssid);
//     IPAddress myIP = WiFi.softAPIP();
//     running = true;
//     server.begin();
// }

// CHSV nextColor() {
//     currentColor = ++currentColor % NUM__COLORS;
//     return colors[currentColor];
// }

// CHSV prevColor() {
//     currentColor = --currentColor % NUM__COLORS;
//     return colors[currentColor];
// }

// void shutDown() {
//     server.end();
//     WiFi.mode(WIFI_OFF);
//     running = false;
// }

// void handleClient(WiFiClient client) {
//     if (client) {                                   // if you get a client,
//         Serial.println("New Client.");              // print a message out the serial port
//         String currentLine = "";                    // make a String to hold incoming data from the client
//         while (client.connected()) {                // loop while the client's connected
//             if (client.available()) {               // if there's bytes to read from the client,
//                 char c = client.read();             // read a byte, then
//                 Serial.write(c);                    // print it out the serial monitor
//                 if (c == '\n') {                    // if the byte is a newline character

//                     // if the current line is blank, you got two newline characters in a row.
//                     // that's the end of the client HTTP request, so send a response:
//                     if (currentLine.length() == 0) {
//                         // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//                         // and a content-type so the client knows what's coming, then a blank line:
//                         client.println("HTTP/1.1 200 OK");
//                         client.println("Content-type:text/html");
//                         client.println();

//                         client.print("<div style=\"display: flex; flex-direction: column; align-items: center; font-size: 16 rem;\">");
//                             client.print("<div style=\"width: 100%; display: flex; flex-direction: column; align-items: center;\">");
//                                 client.print("<p>Color <a href=\"/CH\">_UP_</a> </p>");
//                                 client.print("<p>Color <a href=\"/CL\">DOWN</a> </p>");
//                             client.print("</div>");
//                             client.print("<div style=\"width: 100%; display: flex; flex-direction: column; align-items: center;\">");
//                                 client.print("<p>Brightness <a href=\"/BH\">_UP_</a> </p>");
//                                 client.print("<p>Brightness <a href=\"/BL\">DOWN</a> </p>");
//                             client.print("</div>");
//                         client.print("</div>");

//                         // The HTTP response ends with another blank line:
//                         client.println();
//                         // break out of the while loop:
//                         break;
//                     } else {    // if you got a newline, then clear currentLine:
//                         currentLine = "";
//                     }
//                 } else if (c != '\r') {  // if you got anything else but a carriage return character,
//                     currentLine += c;      // add it to the end of the currentLine
//                 }

//                 if (currentLine.endsWith("GET /CH")) {
//                     CHSV color = nextColor();
//                     reset(color);
//                 }

//                 if (currentLine.endsWith("GET /CL")) {
//                     CHSV color = prevColor();
//                     reset(color);
//                 }

//                 if (currentLine.endsWith("GET /BH")) {
//                     if(g_brightness < maxBrightness)
//                         g_brightness += step;
//                 }

//                 if (currentLine.endsWith("GET /BL")) {
//                     if(g_brightness > minBrightness)
//                         g_brightness -= step;
//                 }
//             }
//         }
//     }
// }

// void copyToCRGB()
// {
//     for(int i = 0; i < NUM__LEDS; i++)
//     {
//         leds[i] = CRGB(hsv_leds[i + WAVE_SIZE]);
//     }
//     for(int i=0; i < NUM__EX_LEDS; i++)
//     {
//         ex_leds[i] = CRGB(hsv_exLeds[i]);
//     }
// }

// int pauseTime = random(5,10);
// bool idle = false;

// int maxNumberOfLoops = NUM__LEDS + WAVE_SIZE;
// int loopCounter = 0;
// bool waveInProgress = true;

// bool pulseInProgress = false;

// int sinValue = 0;

// void dimTo(int value) {
//     for(int i = 0; i < NUM__LEDS; ++i)
//     {
//         if(hsv_leds[i].v >= value) {
//             hsv_leds[i].v -= 5;
//         }
//     }
// }

// void waveEffect() {
//     EVERY_N_MILLISECONDS(200) {
//         for(int i = 0; i < WAVE_SIZE; ++i) {
//             hsv_leds[loopCounter + i].v = 225;
//         }

//         if(loopCounter > 0) {
//             hsv_leds[loopCounter-1].v = 100;
//         }

//         if(loopCounter == maxNumberOfLoops) {
//             loopCounter = 0;
//             waveInProgress = false;
//             pulseInProgress = true;
//         }
//         ++loopCounter;
//     }
// }

// void pulseEffect() {
//     EVERY_N_MILLISECONDS(10)
//     {
//         for(int i = 0; i < NUM__EX_LEDS; ++i)
//         {
//             hsv_exLeds[i].v = sin(sinValue*M_PI/180.f) * 150 + 100;
//         }

//         ++sinValue;

//         if(sinValue == 180) {
//             sinValue = 0;
//             pulseInProgress = false;
//             pauseTime = random(5,10);
//             idle = true;
//         }
//     }
// }
// int MyTimeCounter = 0;
// void loop()
// {
//     if(running && millis() > 3 * 60 * 1000) {
//         shutDown();
//     }

//     WiFiClient client = server.available();   // listen for incoming clients
//     handleClient(client);
//     client.stop();

//     if(waveInProgress) {
//         waveEffect();
//     }

//     if(pulseInProgress) {
//         pulseEffect();
//     }

//     EVERY_N_MILLISECONDS(50) {
//         if(idle) {
//             MyTimeCounter += 50;
//             if(MyTimeCounter >= pauseTime*1000) {
//                 MyTimeCounter = 0;
//                 idle = false;
//                 waveInProgress = true;
//             }
//         }
//     }

//     copyToCRGB();
//     FastLED.setBrightness(g_brightness);
//     FastLED.show();
// }
