// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include <iostream>
#include <set>

namespace mx
{
    namespace impl
    {
        /* return the greatest common divisor
         for a and b.  if both a and b are zero,
         gcd returns zero. if a OR b is zero, the
         absolute value of the non-zero term is
         returned */
        int greatestCommonDivisor( int a, int b );
        
        /* return the greatest common divisor for
         all of the integers in the list.  you should
         only use this for lists of 2 or more non-
         zero numbers.  otherwise... if zero is
         encountered more than once, a std::runtime_error
         is thrown. if zero is encountered once, it does
         not affect the outcome.  if the list has a
         single non-zero term, the absolute value of the
         single term is returned.  if the list has a
         single term which is a zero, 1 is returned */
        int greatestCommonDivisor( std::initializer_list<int> integers );
        
        /* return the least common multiple
         for a and b. if either a or b is
         zero, returns zero. */
        int leastCommonMultiple( int a, int b );
        
        /* don't give this any zeros, if you do
         it will throw a std::runtime_error. If
         you give it an empty list, returns zero.
         if you give it 1 term, it will return the
         absolute value of the term you gave it. */
        int leastCommonMultiple( const std::set<int>& integers );
        
    }
}
