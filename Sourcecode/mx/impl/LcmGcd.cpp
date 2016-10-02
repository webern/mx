// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/LcmGcd.h"
#include "mx/utility/Throw.h"

#include <exception>
#include <cmath>
#include <set>

namespace mx
{
    namespace impl
    {
        int greatestCommonDivisor( int a, int b )
        {
            a = std::abs( a );
            b = std::abs( b );
            // http://stackoverflow.com/a/4229930/2779792
            for (;;)
            {
                if ( a == 0 ) return b;
                b %= a;
                if ( b == 0 ) return a;
                a %= b;
            }
        }
        
        
        int greatestCommonDivisor( std::initializer_list<int> integers )
        {
            int gcd = 1;
            if ( integers.size() == 0 )
            {
                return gcd;
            }
            else if( integers.size() == 1 )
            {
                gcd = std::abs( *( integers.begin() ) );
                return gcd == 0 ? 1 : gcd;
            }
            else
            {
                bool zeroEncountered = false;
                auto iter = integers.begin();
                auto end = integers.end();
                gcd = *iter;
                zeroEncountered = ( gcd == 0 );
                ++iter;
                for (; iter != end; ++iter )
                {
                    if ( *iter == 0 )
                    {
                        if ( zeroEncountered )
                        {
                            MX_THROW("initializer list for gcd function may contain no more than a single 0")
                        }
                        zeroEncountered = true;
                    }
                    gcd = greatestCommonDivisor( gcd, *iter );
                }
                
            }
            return gcd;
        }
        
        
        int leastCommonMultiple( int a, int b )
        {
            // http://stackoverflow.com/a/4229930/2779792
            auto temp = greatestCommonDivisor(a, b);
            
            auto answer = temp ? (a / temp * b) : 0;
            if ( answer < 0 )
            {
                answer *= -1;
            }
            return answer;
        }
        
        
        int leastCommonMultiple( const std::set<int>& integers )
        {
            int answer = 0;
            if ( integers.size() == 0 )
            {
                return 0;
            }
            else if( integers.size() == 1 )
            {
                if ( *( integers.cbegin() ) == 0 )
                {
                    MX_THROW( "zeros not allowed" )
                }
                return std::abs( *( integers.begin() ) );
            }
            else
            {
                auto a = integers.cbegin();
                auto b = integers.cbegin();
                auto e = integers.cend();
                ++b;
                if ( *a == 0 || *b == 0 )
                {
                    MX_THROW( "zeros not allowed" )
                }
                answer = leastCommonMultiple( *a, *b );
                for (; b != e ; ++b )
                {
                    if ( *b == 0 )
                    {
                        MX_THROW( "zeros not allowed")
                    }
                    answer = leastCommonMultiple( answer, *b );
                }
            }
            return answer;
        }
    }
}
