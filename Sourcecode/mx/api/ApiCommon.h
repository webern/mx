// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <cmath>
#include <algorithm>
#include <ostream>
#include <vector>
#include <map>
#include <limits>

namespace mx
{
    namespace api
    {
        constexpr const long double MX_API_EQUALITY_EPSILON = 0.00000001;

        inline bool areSame( long double left, long double right )
        {
            return ( std::abs( left - right ) < MX_API_EQUALITY_EPSILON );
        }

        constexpr int DEFAULT_TICKS_PER_QUARTER = 3 * 4 * 5 * 7;
        constexpr int TICK_TIME_INFINITY = std::numeric_limits<int>::max();

        enum class Bool
        {
            unspecified,
            yes,
            no
        };
        
        enum class Placement
        {
            unspecified,
            above,
            below
        };

        template <typename T>
        inline bool areVectorsEqual( const std::vector<T>& lhs, const std::vector<T>& rhs )
        {
            if( lhs.size() != rhs.size() )
            {
                return false;
            }
            auto lit = lhs.cbegin();
            auto rit = rhs.cbegin();
            auto leftend = lhs.cend();
            for( ; lit != leftend; ++lit, ++rit )
            {
                if( *lit != *rit )
                {
                    return false;
                }
            }
            return true;
        }
        
        template <typename T>
        inline bool areIntMapsEqual( const std::map<int,T>& lhs, const std::map<int,T>& rhs )
        {
            if( lhs.size() != rhs.size() )
            {
                return false;
            }
            auto lit = lhs.cbegin();
            auto rit = rhs.cbegin();
            auto leftend = lhs.cend();
            for( ; lit != leftend; ++lit, ++rit )
            {
                if( lit->first != rit->first || lit->second != rit->second )
                {
                    return false;
                }
            }
            return true;
        }
        
        template <typename T>
        inline bool areEqual( const T& lhs, const T& rhs )
        {
            return lhs == rhs;
        }
        
        template <>
        inline bool areEqual<long double>( const long double& lhs, const long double& rhs )
        {
            return areSame( lhs, rhs );
        }

        
#define MXAPI_EQUALS_BEGIN( mxapiClassName ) \
        inline bool operator==( const mxapiClassName& lhs, const mxapiClassName& rhs ) \
        { \

#define MXAPI_EQUALS_FIRST_MEMBER( mxapiMemberName ) \
            return ( lhs.mxapiMemberName == rhs.mxapiMemberName ) && \

#define MXAPI_EQUALS_NEXT_MEMBER( mxapiMemberName ) \
    ( lhs.mxapiMemberName == rhs.mxapiMemberName ) && \

#define MXAPI_EQUALS_LAST_MEMBER( mxapiMemberName ) \
    ( lhs.mxapiMemberName == rhs.mxapiMemberName ); \

#define MXAPI_EQUALS_END \
    }\


        
        
#define MXAPI_NOT_EQUALS_AND_VECTORS( mxapiClassName ) \
        inline bool operator!=( const mxapiClassName& lhs, const mxapiClassName& rhs ) { return !( lhs == rhs ); } \
        inline bool operator==( const std::vector<mxapiClassName>& lhs, const std::vector<mxapiClassName>& rhs ) { return areVectorsEqual<mxapiClassName>( lhs, rhs ); } \
        inline bool operator!=( const std::vector<mxapiClassName>& lhs, const std::vector<mxapiClassName>& rhs ) { return !areVectorsEqual<mxapiClassName>( lhs, rhs ); } \

#define MXAPI_INT_MAPS_EQUALS( mxapiClassName ) \
        inline bool operator==( const std::map<int,mxapiClassName>& lhs, const std::map<int,mxapiClassName>& rhs ) { return areIntMapsEqual<mxapiClassName>( lhs, rhs ); } \
        inline bool operator!=( const std::map<int,mxapiClassName>& lhs, const std::map<int,mxapiClassName>& rhs ) { return !areIntMapsEqual<mxapiClassName>( lhs, rhs ); } \

    }
}
