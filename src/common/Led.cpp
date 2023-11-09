#include "Led.h"

#include "Logger.h"

// Arduino:
#include <Arduino.h>


Led::Led()
{
    // setup LED:
    Log( "Activating built-in LED..." );
    pinMode( LED_BUILTIN, OUTPUT );

    // disable by default:
    Disable();
}


void Led::Enable( const bool _enable )
{
    m_enabled = _enable;
    digitalWrite( LED_BUILTIN, m_enabled ? LOW : HIGH );
}


void Led::Disable()
{
    Enable( false );
}


void Led::Toggle()
{
    Enable( !m_enabled );
}