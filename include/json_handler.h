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

    ParsedAnimationData Read()
    {
        ParsedAnimationData retData;

        if (!SPIFFS.begin(true))
        {
            Serial.println("An Error has occurred while mounting SPIFFS");
        }

        File file = SPIFFS.open("/settings.json");
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

        StaticJsonDocument<2000> doc;

        DeserializationError error = deserializeJson(doc, jsonFileData);

        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());

            // handle error!!! switch to default
        }

        retData.brightnes = doc["brightness"]; // 90

        JsonObject baseColor = doc["baseColor"];

        retData.baseColor.H = baseColor["h"]; // 160
        retData.baseColor.S = baseColor["s"]; // 160
        retData.baseColor.V = baseColor["v"]; // 64

        for (JsonObject effect : doc["effects"].as<JsonArray>())
        {
            ParsedEffectData effData;

            effData.effectColor = DCHSV(effect["h"], effect["s"], effect["v"]);

            effData.effectDelta.H = effect["h"];
            effData.effectDelta.S = effect["s"];
            effData.effectDelta.V = effect["v"];

            effData.effectDelta.deltaH = effect["h_d"];
            effData.effectDelta.deltaS = effect["s_d"];
            effData.effectDelta.deltaV = effect["v_d"];
            effData.effectDelta.chance = effect["c_c"];
            effData.effectDelta.weight = effect["w"];

            for (JsonObject ratio : effect["ratios"].as<JsonArray>())
            {
                ParsedEffectPartData epData = ParsedEffectPartData();

                epData.ratio = ratio["r"];
                epData.typeId = ratio["typeId"];
                epData.minTime = ratio["minT"];
                epData.maxTime = ratio["maxT"];
                Serial.print(epData.typeId);

                effData.partDatas.push_back(epData);
            }
            Serial.println(" r ");

            retData.effectDatas.push_back(effData);
        }
        return retData;
    }

    bool Write(ParsedEffectData data)
    {
        return true;
    }
};
