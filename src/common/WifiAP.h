#pragma once

// Arduino:
#include <ESP8266WiFi.h>

// std:
#include <string>
#include <functional>

// external:
#include "external/DNSServer.h"


class WifiAP
{
public:
    struct Parameters;

    struct Callbacks
    {
        const std::function< void() > noDevice;
        const std::function< void() > deviceConnected;
        const std::function< void() > deviceDisconnected;
    };

public:
    WifiAP( const Parameters & _parameters, Callbacks && _callbacks );
    ~WifiAP() = default;

public:
    void Loop();

private:
    const Callbacks m_callbacks;
    DNSServer m_dnsServer;
    int m_connectedStations{ 0 };
};


struct WifiAP::Parameters
{
    const std::string ssid;
    const std::string password;
    const IPAddress ip;
    const std::string domain;
};