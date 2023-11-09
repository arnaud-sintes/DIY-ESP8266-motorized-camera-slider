#pragma once

#include "Enum.h"
#include "Utils.h"

// Arduino:
#include <ESP8266WebServer.h>

// std:
#include <string>
#include <list>
#include <unordered_map>
#include <functional>
#include <optional>
#include <memory>


class WebServer
{
public:
    struct Parameters;
    struct Service;

public:
    using Services = std::list< Service >;
    WebServer( const Parameters & _parameters, const Services & _services );
    
    ~WebServer() = default;

public:
    enum eStatus : int
    {
        ok = 200,
        notFound = 404, // resource not found
        unprocessableEntity = 422, // request semantic errors
        internalServerError = 500, // generic server error
    };

    static Enum< eStatus > & Statuses();

public:
    void Loop();

private:
    ESP8266WebServer m_server;
};


struct WebServer::Parameters
{
    const int port;
};


struct WebServer::Service
{
    const std::string route;
    
    using RawParameters = std::unordered_map< std::string, std::string >;
    
    class IParameter;

    template< typename _Type >
    class ParameterT;
    
    template< typename _Type >
    class Parameter;
    
    using IParameterPtr = std::unique_ptr< IParameter >;
    using Parameters = std::unordered_map< std::string, IParameterPtr >;
    const Parameters & parameters;

    struct Response;
    using WorkerFn = std::function< Response( const Parameters & ) >;
    const WorkerFn & worker;
};


struct WebServer::Service::Response
{
    const eStatus status{ eStatus::ok };
    const std::string data{ "" };
};


class WebServer::Service::IParameter
{
    friend std::unique_ptr< IParameter >::deleter_type;

protected:
    IParameter() = default;
    virtual ~IParameter() = default;

public:
    template< typename _Type >
    const std::optional< _Type > & Get() const
    {
        return static_cast< const ParameterT< _Type > & >( *this ).Get();
    }

public:
    virtual std::optional< std::string > Validate( const std::string & _value );
};


template< typename _Type >
class WebServer::Service::ParameterT
    : public IParameter
{
protected:
    ParameterT() = default;
    virtual ~ParameterT() = default;

public:
    const std::optional< _Type > & Get() const { return m_value; }

protected:
    void Set( const _Type _value ) { m_value = _value; }

private:
    std::optional< _Type > m_value;
};


template<>
class WebServer::Service::Parameter< unsigned >
    : public ParameterT< unsigned >
{
public:
    Parameter( const std::optional< int > & _min = {}, const std::optional< int > & _max = {} )
        : m_min{ _min }
        , m_max{ _max }
    {}

    ~Parameter() = default;

public:
    std::optional< std::string > Validate( const std::string & _value ) final
    {
        const auto value{ Utils::ToUnsigned( _value ) };
        if( !value )
            return { "value must be an unsigned number (current: " + _value + ")" };
        if( m_min && static_cast< int >( *value ) < *m_min )
            return { "value must be greater or equal than " + std::to_string( *m_min ) + " (current: " + _value + ")" };
        if( m_max && static_cast< int >( *value ) > *m_max )
            return { "value must be lesser or equal than " + std::to_string( *m_max ) + " (current: " + _value + ")" };
        ParameterT< unsigned >::Set( static_cast< unsigned >( *value ) );
        return {};
    }

private:
    const std::optional< int > m_min;
    const std::optional< int > m_max;
};


template<>
template< typename _Type >
class WebServer::Service::Parameter< Enum< _Type > >
    : public ParameterT< _Type >
{
public:
    Parameter( const Enum< _Type > & _enum )
        : m_enum{ _enum }
    {}

    ~Parameter() = default;

public:
    std::optional< std::string > Validate( const std::string & _value ) final
    {
        const auto value{ m_enum.Get( _value ) };
        if( !value )
            return { "value type '" + _value + "' is unknown" };
        ParameterT< _Type >::Set( *value );
        return {};
    }

private:
    const Enum< _Type > & m_enum;
};