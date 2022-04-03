#pragma once

#include "WiFi.h"
#include <WebServer.h>

#include "SPIFFS.h"

WebServer server(80);

const char *ssid = "ASUS_80";
const char *password = "keeper_4664";

IPAddress local_IP = IPAddress(192, 168, 50, 95);
IPAddress gateway = IPAddress(192, 168, 50, 1);

IPAddress subnet = IPAddress(255, 255, 0, 0);
IPAddress primaryDNS = IPAddress(8, 8, 8, 8);
IPAddress secondaryDNS = IPAddress(8, 8, 4, 4);

void turnOn()
{
    renderer.Activate();
    server.send(200, "text/plain", "YO");
}

void turnOff()
{
    renderer.Deactivate();
    server.send(200, "text/plain", "Swag");
}

void setBrightness()
{
    server.send(200, "text/plain", "DESU");
    int arg = server.arg(0).toInt();
    if (arg < 0)
        arg = 0;
    if (arg > 255)
        arg = 255;

    renderer.SetBrightness(arg);
    server.send(200, "text/plain", "shine");
}

void setColor()
{

    StaticJsonDocument<60> doc;

    DeserializationError error = deserializeJson(doc, server.arg(0));

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());

        // handle error!!! switch to default
    }

    Serial.print("setcol -  h: ");
    Serial.print(doc["h"].as<String>());
    Serial.print(" s: ");
    Serial.print(doc["s"].as<String>());
    Serial.print(" v: ");
    Serial.print(doc["v"].as<String>());
    Serial.println();

    animHandler.baseEffects[0]->_c = DCHSV(doc["h"], doc["s"], doc["v"]);
    server.send(200, "text/plain", "Nice!");
}

void resetAnimation()
{
    ParsedAnimationData animations = jsonHandler.Read();
    animHandler.Reset();
    animHandler.Setup(&renderer, &animations);
}

void saveFile()
{
    for (int i = 0; i < server.args(); i++)
    {
        Serial.println(server.arg(i));

        File file = SPIFFS.open("/settings.json", FILE_WRITE);

        if (!file)
        {
            Serial.println("There was an error opening the file for writing");
            server.send(500, "text/plain", "fffFFFFf....");
            return;
        }

        file.print(server.arg(0));

        file.close();

        server.send(200, "text/plain", "YO");
    }

    resetAnimation();
}

void resetConfigToDefault()
{
    File target = SPIFFS.open("/settings.json", FILE_WRITE);
    File source = SPIFFS.open("/default.json", FILE_READ);

    if (!target || !source)
    {
        Serial.println("There was an error opening the file for writing");
        server.send(500, "text/plain", "crap");
        return;
    }

    target.print(source.read());

    target.close();
    source.close();

    server.send(200, "text/plain", "Fasza!");
}

bool ServerSetup()
{
    server.serveStatic("/", SPIFFS, "/index.html");
    server.serveStatic("/antique.ttf", SPIFFS, "/antique.ttf");
    server.serveStatic("/settings.json", SPIFFS, "/settings.json");
    server.serveStatic("/default.json", SPIFFS, "/default.json");

    server.on(F("/off"), HTTP_GET, turnOff);
    server.on(F("/on"), HTTP_GET, turnOn);
    server.on(F("/setbright"), HTTP_GET, setBrightness);
    server.on(F("/reset"), HTTP_GET, resetConfigToDefault);

    server.on(F("/setcolor"), HTTP_POST, setColor);
    server.on(F("/savesettings"), HTTP_POST, saveFile);

    server.begin();

    Serial.println("HTTP server started");

    return true;
}

bool WifiSetup()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
        return false;
    }

    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    if (!ServerSetup())
    {
        return false;
    }

    return true;
}
