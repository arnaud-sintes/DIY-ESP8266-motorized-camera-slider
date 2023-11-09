#include "ServoMotor.h"

#include "Logger.h"

// std:
#include <string>


ServoMotor::ServoMotor( const Parameters & _parameters )
    : m_pivot{ _parameters.pivot }
{
    Log( "Attaching servomotor on pin " + std::to_string( _parameters.pin ) + "..." );

    // attach servo motor to given pin:
    m_servo.attach( _parameters.pin );

    // stop by default:
    Log( "Stopping servomotor..." );
    Stop();   
}


Enum< ServoMotor::eDirection > & ServoMotor::Directions()
{
    static Enum< eDirection > directions{ {
        EnumValue( eDirection, forward ),
        EnumValue( eDirection, backward ),
    } };
    return directions;
}


void ServoMotor::Stop()
{
    // rotation of null speed:
    Rotate( eDirection::forward, 0 );
}


void ServoMotor::Rotate( const eDirection _direction, const unsigned _speed )
{
    // value depend on direction:
    int value{ static_cast< int >( _speed ) };
    if( _direction == eDirection::forward )
        value += m_pivot;
    else
        value = m_pivot - value;
    _Set( value );
}


void ServoMotor::_Set( const int _value )
{
    // write servo value:
    m_servo.write( _value );
}