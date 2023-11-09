#pragma once

// std:
#include <list>
#include <unordered_map>
#include <string>


template< typename _Type >
class Enum
{
public:
    Enum( std::list< std::pair< _Type, std::string > > && _elements )
    {
        for( auto && element : _elements ) {
            m_type2name.emplace( element.first, element.second );
            m_name2type.emplace( element.second, element.first );
        }
    }

    ~Enum() = default;

public:
    const std::string & Get( const _Type _type ) const
    {
        return m_type2name.find( _type )->second;
    }

    bool Exists( const std::string & _name ) const
    {
        return m_name2type.find( _name ) != m_name2type.cend();
    }

    std::optional< _Type > Get( const std::string & _name ) const
    {
        auto itType{ m_name2type.find( _name ) };
        if( itType == m_name2type.cend() )
            return {};
        return itType->second;
    }

private:
    std::unordered_map< _Type, std::string > m_type2name;
    std::unordered_map< std::string, _Type > m_name2type;
};


#define EnumValue( _enum, _value ) { _enum::_value, "" # _value }