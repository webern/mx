// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include <algorithm>
#include <ostream>
#include <vector>
#include <map>
#include <iostream>

namespace mx
{
    namespace api
    {
        
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All of the uglines below this point is mainly to be used in test code.  It is not expected that clients will have need of the //
// functions defined here.  If you do wish to use the equality operator, you may want to silence the cout's that occur when two  //
// items are found to be not-equal.  This cout stream was necessary for testing during development.                              //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

        inline void streamComparisonUnequalMessage( const char* const inClassName, const char* const inMemberName )
        {
            std::cout << inClassName;
            std::cout << " ";
            std::cout << inMemberName;
            std::cout << " members are not equal ";
            std::cout << std::endl;
        }
        
#define MXAPI_EQUALS_BEGIN( mxapiClassName ) \
        inline bool operator==( const mxapiClassName& lhs, const mxapiClassName& rhs ) \
        { \
            const char* const theCurrentClassName = #mxapiClassName;

#define MXAPI_EQUALS_MEMBER( mxapiMemberName ) \
        if( ! ( lhs.mxapiMemberName == rhs.mxapiMemberName ) ) \
        { \
            streamComparisonUnequalMessage( theCurrentClassName, #mxapiMemberName ); \
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
