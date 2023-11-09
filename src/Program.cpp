#include "Program.h"


void Program::Setup( Parameters && _parameters )
{
    // setup logger:
    Logger::Instance().Init( _parameters.logger );
    Log( "Setup program..." );

    // setup servo motor:
    m_servoMotor = std::make_unique< ServoMotor >( _parameters.servoMotor );

    // setup wifi access point:
    m_wifiAP = std::make_unique< WifiAP >( _parameters.wifiAP, WifiAP::Callbacks{
        [ this ]{
            // make the LED blink when waiting for device connection:
            m_led.Toggle();
            delay( 500 );
        },
        [ this ]{
            Log( "Device connected to access point." );
            m_led.Enable();
        },
        []{
            Log( "Device diconnected from access point." );
        }
    } );

    // setup "set" service:
    auto & service_set{ m_parameters.emplace( "set", ServiceParameters{ [ this ]( const auto & _parameters ) { return _Service_Set( _parameters ); } } ).first->second };
    service_set.parameters.emplace( "direction", std::make_unique< WebServer::Service::Parameter< Enum< ServoMotor::eDirection > > >( ServoMotor::Directions() ) );
    service_set.parameters.emplace( "speed", std::make_unique< WebServer::Service::Parameter< unsigned > >( 0, 90 ) );

    // build services:
    for( const auto & parameter : m_parameters )
        m_services.emplace_back( WebServer::Service{ parameter.first, parameter.second.parameters, parameter.second.workerFn } );
    
    // setup webserver with all services:
	m_webServer = std::make_unique< WebServer >( _parameters.webServer, m_services );

    Log( "Setup program done." );
}


WebServer::Service::Response Program::_Service_Set( const WebServer::Service::Parameters & _parameters )
{
    // rotate servo motor:
    const auto direction{ _parameters.find( "direction" )->second->Get< ServoMotor::eDirection >() };
    const auto speed{ _parameters.find( "speed" )->second->Get< unsigned >() };
    m_servoMotor->Rotate( *direction, *speed );
    return {};
}


void Program::Loop()
{
    m_wifiAP->Loop();
    m_webServer->Loop();
}