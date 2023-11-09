#pragma once

// std:
#include <string>


class Logger
{
public:
    struct Parameters;

private:
    Logger() = default;
    ~Logger() = default;

public:
    static Logger & Instance()
    {
        static Logger instance;
        return instance;
    }

public:
    void Init(  const Parameters & _parameters  );

    enum class eType
    {
        info,
        error,
        debug,
    };

    void LogLine( const eType _eType, const std::string & _message );
};


struct Logger::Parameters
{
    const int baudRate;
};


#define Log( _message ) Logger::Instance().LogLine( Logger::eType::info, _message )
#define LogError( _message ) Logger::Instance().LogLine( Logger::eType::error, _message )
#define LogDebug( _message ) Logger::Instance().LogLine( Logger::eType::debug, _message )