#include "WifiAP.h"

#include "Logger.h"


WifiAP::WifiAP( const Parameters & _parameters, Callbacks && _callbacks )
    : m_callbacks{ std::move( _callbacks ) }
{
    // setup Access Point mode:
    Log( "Startup WiFi access point '" + _parameters.ssid + "'..." );
    WiFi.mode( WIFI_AP );
    WiFi.softAPConfig( _parameters.ip, _parameters.ip, IPAddress( 255, 255, 255, 0 ) );
    WiFi.softAP( _parameters.ssid.c_str(), _parameters.password.c_str(), 1, false, 1 );

    // dump local ip:
    Log( "Access point ready, local IP is " + std::string{ WiFi.softAPIP().toString().c_str() } );

    // set DNS server:
    m_dnsServer.setTTL( 60 );
    m_dnsServer.setErrorReplyCode( DNSReplyCode::ServerFailure );
    m_dnsServer.start( 53, _parameters.domain.c_str(), _parameters.ip );
    Log( "DNS server ready, domain is '" + _parameters.domain + "'" );
}


void WifiAP::Loop()
{
    // dns server:
    m_dnsServer.processNextRequest();

    // how many connected stations:
    int connectedStations{ WiFi.softAPgetStationNum() };

    // no status change:
    if( m_connectedStations == connectedStations ) {

        // already connected:
        if( m_connectedStations == 1 )
            return;

        // no-one is connected:
        return m_callbacks.noDevice();
    }

    // there's a device connection change:
    m_connectedStations = connectedStations;

    // device connection:
    if( m_connectedStations == 1 )
        return m_callbacks.deviceConnected();

    // device disconnection:
    m_callbacks.deviceDisconnected();
}