#include "Utils.h"

// std:
#include <algorithm>


bool Utils::IsUnsigned( const std::string & _value )
{
    return !_value.empty() && std::find_if( _value.cbegin(), _value.cend(),
        []( unsigned char _c ) {
            return !std::isdigit( _c );
        } ) == _value.end();
}


std::optional< unsigned long > Utils::ToUnsigned( const std::string & _value )
{
    if( !IsUnsigned( _value ) )
        return {};
    return std::stoul( _value );
}