#pragma once

#include "common/Utils.h"
#include "common/Led.h"
#include "common/Logger.h"
#include "common/ServoMotor.h"
#include "common/WifiAP.h"
#include "common/WebServer.h"

// std:
#include <memory>
#include <unordered_map>


class Program
{
public:
    struct Parameters;

public:
    Program() = default;
    ~Program() = default;

public:
	void Setup( Parameters && _parameters );
    void Loop();

private:
    WebServer::Service::Response _Service_Set( const WebServer::Service::Parameters & _parameters );

private:
    Led m_led;
    std::unique_ptr< ServoMotor > m_servoMotor;
    std::unique_ptr< WifiAP > m_wifiAP;
    std::unique_ptr< WebServer > m_webServer;

    struct ServiceParameters
    {
        const WebServer::Service::WorkerFn workerFn;
        WebServer::Service::Parameters parameters;
    };
    std::unordered_map< std::string, ServiceParameters > m_parameters;
    WebServer::Services m_services;
};


struct Program::Parameters
{
    Logger::Parameters logger;
    ServoMotor::Parameters servoMotor;
    WifiAP::Parameters wifiAP;
    WebServer::Parameters webServer;
};