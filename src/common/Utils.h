#pragma once

// std:
#include <string>
#include <optional>


struct Utils
{
    static bool IsUnsigned( const std::string & _value );
    static std::optional< unsigned long > ToUnsigned( const std::string & _value );
};