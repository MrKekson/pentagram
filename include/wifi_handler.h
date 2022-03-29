#pragma once

#include "WiFi.h"
#include <WebServer.h>

#include "SPIFFS.h"

WebServer server(80);

const char *ssid = "ASUS_80";
const char *password = "keeper_4664";

// testing! delete if ap mode is on
// Set your Static IP address
IPAddress local_IP = IPAddress(192, 168, 50, 95);
// Set your Gateway IP address
IPAddress gateway = IPAddress(192, 168, 50, 1);

IPAddress subnet = IPAddress(255, 255, 0, 0);
IPAddress primaryDNS = IPAddress(8, 8, 8, 8);   // optional
IPAddress secondaryDNS = IPAddress(8, 8, 4, 4); // optional

void pathRoot()
{
    server.send(200, "text/plain", "hello from esp32!");
}

bool ServerSetup()
{
    server.serveStatic("/json/get", SPIFFS, "/settings.json");
    server.serveStatic("/main.html", SPIFFS, "/main.html");
    server.serveStatic("/src", SPIFFS, "/src");
    server.begin();

    Serial.println("HTTP server started");

    return true;
}

bool WifiSetup()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
        return false;
    }

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
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
