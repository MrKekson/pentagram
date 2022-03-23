#pragma once

#include "SPIFFS.h"
#include <ArduinoJson.h>

#include "base.h"

class JsonHandler
{
public:
    bool ResetToDefault()
    {
        if (!SPIFFS.begin(true))
        {
            Serial.println("An Error has occurred while mounting SPIFFS");
            return false;
        }

        File sourceFile = SPIFFS.open("/base.json");
        if (!sourceFile)
        {
            Serial.println("Failed to open source");
            return false;
        }

        File targetFile = SPIFFS.open("/base.json");
        if (!targetFile)
        {
            Serial.println("Failed to open target");
            return false;
        }

        return true;
    }

    ParsedEffectData Read()
    {

        if (!SPIFFS.begin(true))
        {
            Serial.println("An Error has occurred while mounting SPIFFS");
        }

        File file = SPIFFS.open("/default.json");
        if (!file)
        {
            Serial.println("Failed to open file for reading");
        }

        Serial.println("File Content:");
        size_t fSize = file.size();

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

        StaticJsonDocument<512> doc;

        DeserializationError error = deserializeJson(doc, jsonFileData);

        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
        }

        ParsedEffectData eData = ParsedEffectData();

        eData.brightness = doc["brightness"]; // 90

        JsonObject baseColor = doc["baseColor"];
        eData.baseColor.h = baseColor["h"]; // 160
        eData.baseColor.s = baseColor["s"]; // 160
        eData.baseColor.v = baseColor["v"]; // 64

        for (JsonObject effect : doc["effects"].as<JsonArray>())
        {

            JsonObject effect_effectColor = effect["effectColor"];
            int effect_effectColor_h = effect_effectColor["h"]; // 180, 140, 80
            int effect_effectColor_s = effect_effectColor["s"]; // 160, 160, 160
            int effect_effectColor_v = effect_effectColor["v"]; // 64, 64, 64
        }
        return eData;
    }

    bool Write(ParsedEffectData data)
    {
        return true;
    }
};
