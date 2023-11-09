#include "WebServer.h"

#include "Logger.h"

// Arduino:
#include <LittleFS.h>


WebServer::WebServer( const Parameters & _parameters, const Services & _services )
    : m_server{ _parameters.port }
{
    // static file service:
    Log( "Startup static file server..." );
    LittleFS.begin();
    m_server.serveStatic( "/", LittleFS, "/" );
    
    // resource not found:
    m_server.onNotFound( [ this ]{
        m_server.send( eStatus::notFound, "text/plain", ( "Resource '" + std::string{ m_server.uri().c_str() } + "' not found!" ).c_str() );
    } );

    // setup services:
    Log( "Startup services..." );
    for( const auto & service : _services ) {
        
        // HTTP POST routing:
        Log( "Registering service '" + service.route + "'..." );
        m_server.on( ( "/" + service.route ).c_str(), HTTP_POST,
            [ this, &service ] {
                Log( "Receiving HTTP POST /set" );

                // extract request raw parameters:
                Service::RawParameters rawParameters;
                for( int i{ 0 }; i < m_server.args(); i++ )
                    rawParameters.emplace( m_server.argName( i ).c_str(), m_server.arg( i ).c_str() );

                // raw parameters validation:
                for( const auto & parameter : service.parameters ) {

                    // missing parameters:
                    auto itRawParameter{ rawParameters.find( parameter.first ) };
                    if( itRawParameter == rawParameters.cend() ) {
                        const std::string message{ "parameter '" + parameter.first + "' is expected" };
                        LogError( message );
                        m_server.send( eStatus::unprocessableEntity, "text/plain", message.c_str() );
                        return;
                    }

                    // inner type validation:
                    const auto error{ parameter.second->Validate( itRawParameter->second ) };
                    if( error ) {
                        const std::string message{ "parameter '" + parameter.first + "' validation error: " + *error };
                        LogError( message );
                        m_server.send( eStatus::unprocessableEntity, "text/plain", message.c_str() );
                        return;
                    }
                }
                
                // call worker to build the response:
                const auto response{ service.worker( service.parameters ) };
                m_server.send( response.status, "text/plain", response.data.c_str() );
            } );
    }

    // start server:
    m_server.begin();
    Log( "Server listening on port " + std::to_string( _parameters.port ) + "..." );
}


Enum< WebServer::eStatus > & WebServer::Statuses()
{
    static Enum< eStatus > statuses{ {
        EnumValue( eStatus::ok ),
        EnumValue( eStatus::notFound ),
        EnumValue( eStatus::unprocessableEntity ),
        EnumValue( eStatus::internalServerError ),
    } };
    return statuses;
}


void WebServer::Loop()
{
    // deal with client's requests:
    m_server.handleClient();
}