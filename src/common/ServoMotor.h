#pragma once

#include "Enum.h"

// Arduino:
#include <Servo.h>


class ServoMotor
{
public:
    struct Parameters;

public:
    ServoMotor( const Parameters & _parameters );
    ~ServoMotor() = default;

public:
    enum class eDirection
    {
        forward,
        backward,
    };

    static Enum< eDirection > & Directions();

public:
    void Stop();

    // speed between [1,90]
    void Rotate( const eDirection _direction, const unsigned _speed );

private:
    void _Set( const int _value );

private:
    Servo m_servo;
};


struct ServoMotor::Parameters
{
    const int pin;
};