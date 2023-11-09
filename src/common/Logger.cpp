#include "Logger.h"

#include "Enum.h"

// Arduino:
#include <Arduino.h>


void Logger::Init( const Parameters & _parameters )
{
    Serial.begin( _parameters.baudRate );
}


void Logger::LogLine( const eType _eType, const std::string & _message )
{
    static Enum< eType > prefixes{ {
        { eType::info, "" },
        { eType::error, "[ERROR] " },
        { eType::debug, "{debug} " },
    } };
    Serial.println( ( prefixes.Get( _eType ) + _message ).c_str() );
}