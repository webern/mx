// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <string>

namespace mx
{
    namespace core
    {
        /* alias for the int size used by this library */
        using IntType = int;
        
        class Integer
        {
        public:
            explicit Integer( IntType value );
            Integer();
            virtual ~Integer();
            IntType getValue() const;
            virtual void setValue( IntType value );
            virtual void parse( const std::string& value );
        private:
            IntType myValue;
        };
        
        
        std::string toString( const Integer& value );
        std::ostream& toStream( std::ostream& os, const Integer& value );
        std::ostream& operator<<( std::ostream& os, const Integer& value );
        
        
        class IntRange : public Integer
        {
        public:
            explicit IntRange( IntType min, IntType max, IntType value );
            virtual ~IntRange();
            IntRange( const IntRange& ) = default;
            IntRange( IntRange&& );
            IntRange& operator=( const IntRange& );
            IntRange& operator=( IntRange&& );
            
            virtual void setValue( IntType value );
            virtual void parse( const std::string& value );
        private:
            const IntType myMin;
            const IntType myMax;
        };
        
        
        class PositiveInteger: public Integer
        {
        public:
            explicit PositiveInteger( IntType value );
            PositiveInteger();
            virtual ~PositiveInteger();
            virtual void setValue( IntType value );
            virtual void parse( const std::string& value );
        };
        
        
        class NonNegativeInteger: public Integer
        {
        public:
            explicit NonNegativeInteger( IntType value );
            NonNegativeInteger();
            virtual ~NonNegativeInteger();
            virtual void setValue( IntType value );
            virtual void parse( const std::string& value );
        };
        
        
		std::string toString( const Integer& value );
		std::ostream& toStream( std::ostream& os, const Integer& value );
		std::ostream& operator<<( std::ostream& os, const Integer& value );
        
        
        // The specification restrics the values from 1 to 3
        // This seems incorrect when searching accordion symbols
        // online so this library will support 0 to 3 instead.
        /* MIN = 0, MAX = 3, DEFAULT = 0 */
        class AccordionMiddleValue : public IntRange
        {
        public:
            explicit AccordionMiddleValue( IntType value );
            AccordionMiddleValue();
        };
        
        
        /* MIN = 1, MAX = 8, DEFAULT = 1 */
        class BeamLevel : public IntRange
        {
        public:
            explicit BeamLevel( IntType value );
            BeamLevel();
        };
        
        
        /* MIN = N/A, MAX = N/A, DEFAULT = 0 */
        class FifthsValue : public Integer
        {
        public:
            explicit FifthsValue( IntType value );
            FifthsValue();
        };
        
        
        /* MIN = 1, MAX = 16, DEFAULT = 1 */
        class Midi16 : public IntRange
        {
        public:
            explicit Midi16( IntType value );
            Midi16();
        };
        
        
        /* MIN = 1, MAX = 128, DEFAULT = 1 */
        class Midi128 : public IntRange
        {
        public:
            explicit Midi128( IntType value );
            Midi128();
        };
        
        
        /* MIN = 1, MAX = 16384, DEFAULT = 1 */
        class Midi16384 : public IntRange
        {
        public:
            explicit Midi16384( IntType value );
            Midi16384();
        };
        
        
        /* MIN = 1, MAX = 6, DEFAULT = 1 */
        class NumberLevel : public IntRange
        {
        public:
            explicit NumberLevel( IntType value );
            NumberLevel();
        };
        
        
        /* MIN = 0, MAX = 3, DEFAULT = 0 */
        class NumberOfLines : public IntRange
        {
        public:
            explicit NumberOfLines( IntType value );
            NumberOfLines();
        };
        
        
        /* MIN = 0, MAX = 9, DEFAULT = 0 */
        class OctaveValue : public IntRange
        {
        public:
            explicit OctaveValue( IntType value );
            OctaveValue();
        };
        
        
        /* MIN = N/A, MAX = N/A, DEFAULT = 0 */
        class StaffLine : public Integer
        {
        public:
            explicit StaffLine( IntType value );
            StaffLine();
        };
        
        
        /* MIN = 1, MAX = N/A, DEFAULT = 1 */
        using StaffNumber = PositiveInteger;
        
        
        /* MIN = 1, MAX = N/A, DEFAULT = 1 */
        using StringNumber = PositiveInteger;
        
        
        /* MIN = 0, MAX = 8, DEFAULT = 0 */
        class TremoloMarks : public IntRange
        {
        public:
            explicit TremoloMarks( IntType value );
            TremoloMarks();
        };
        
    }
}
