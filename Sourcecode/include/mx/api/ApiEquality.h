// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include <algorithm>
#include <ostream>
#include <vector>
#include <map>
#include <iostream>

// #define MX_DEBUG // shows traces to std::cout

namespace mx
{
    namespace api
    {
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

#ifdef MX_DEBUG
        inline void streamComparisonUnequalMessage( const char* const inClassName, const char* const inMemberName )
        {
            std::cout << inClassName;
            std::cout << " ";
            std::cout << inMemberName;
            std::cout << " members are not equal ";
            std::cout << std::endl;
        }
#endif

#ifdef MX_DEBUG
    #define MX_SHOW_UNEQUAL( XtheCurrentClassName, XmxapiMemberName ) streamComparisonUnequalMessage( XtheCurrentClassName, XmxapiMemberName );
#else
    #define MX_SHOW_UNEQUAL( XtheCurrentClassName, XmxapiMemberName )
#endif

#define MXAPI_EQUALS_BEGIN( mxapiClassName ) \
        inline bool operator==( const mxapiClassName& lhs, const mxapiClassName& rhs ) \
        { \
            const char* const theCurrentClassName = #mxapiClassName;
            
#define MXAPI_EQUALS_MEMBER( mxapiMemberName ) \
        if( ! ( lhs.mxapiMemberName == rhs.mxapiMemberName ) ) \
        { \
            MX_SHOW_UNEQUAL( theCurrentClassName, #mxapiMemberName ); \
            return false; \
        }

#define MXAPI_DOUBLES_EQUALS_MEMBER( mxapiMemberName ) \
        if( std::abs( lhs.mxapiMemberName - rhs.mxapiMemberName ) > MX_API_EQUALITY_EPSILON ) \
        { \
            MX_SHOW_UNEQUAL( theCurrentClassName, #mxapiMemberName ); \
            return false; \
        }

#define MXAPI_EQUALS_END \
        return true; \
    }\
        
#define MXAPI_NOT_EQUALS_AND_VECTORS( mxapiClassName ) \
        inline bool operator!=( const mxapiClassName& lhs, const mxapiClassName& rhs ) { return !( lhs == rhs ); } \
        inline bool operator==( const std::vector<mxapiClassName>& lhs, const std::vector<mxapiClassName>& rhs ) { return areVectorsEqual<mxapiClassName>( lhs, rhs ); } \
        inline bool operator!=( const std::vector<mxapiClassName>& lhs, const std::vector<mxapiClassName>& rhs ) { return !areVectorsEqual<mxapiClassName>( lhs, rhs ); } \

    }
}
