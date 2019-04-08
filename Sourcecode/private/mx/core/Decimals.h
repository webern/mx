// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <cmath>

namespace mx
{
    namespace core
    {
        
        /* alias for the floating point size used by this library.
         Note, "decimal" is synonymous with "float" in this context. */
        using DecimalType = long double;
        
        /* decimals will be rounded to this number of places,
           you must recompile if this number is changed */
        const unsigned short DEFAULT_PRECISION = 6;
        
        /* when a Decimal's range is Exclusive and you try to assign
         out-of-range, this amount will be added to the exclusive minimum.
         For example if a Decimal is constrainted to be a positive number,
         and you try to assign the value 0, the value will by set to 
         NON_ZERO_AMOUNT instead */
        const DecimalType NON_ZERO_AMOUNT = 0.000001;
        
        class Decimal
        {
        public:
            explicit Decimal( DecimalType value );
            Decimal();
            virtual ~Decimal();
            Decimal( const Decimal& other );
            Decimal( Decimal&& other );
            Decimal& operator=( const Decimal& other );
            Decimal& operator=( Decimal&& other );
            
            DecimalType getValue() const;
            virtual void setValue( DecimalType value );
            virtual void parse( const std::string& value );
            std::string toString() const;
            std::ostream& toStream( std::ostream& os ) const;

        private:
            class impl;
            std::unique_ptr<impl> myImpl;
        };
        
        std::string toString( const Decimal& value, unsigned int precision = DEFAULT_PRECISION );
		std::ostream& toStream( std::ostream& os, const Decimal& value, unsigned int precision = DEFAULT_PRECISION );
		std::ostream& operator<<( std::ostream& os, const Decimal& value );
        
        class DecimalRange : public Decimal
        {
        public:
            explicit DecimalRange( DecimalType min, DecimalType max, DecimalType value );
            // virtual ~DecimalRange();
            DecimalRange( const DecimalRange& other ) = default;
            DecimalRange( DecimalRange&& other );
            DecimalRange& operator=( const DecimalRange& other );
            DecimalRange& operator=( DecimalRange&& other );
            virtual void setValue( DecimalType value );
            virtual void parse( const std::string& value );
        private:
            const DecimalType myMin;
            const DecimalType myMax;
        };
        
        /* MIN = 0 EXCLUSIVE!, MAX = N/A, DEFAULT = 1 */
        class PositiveDecimal: public Decimal
        {
        public:
            explicit PositiveDecimal( DecimalType value );
            PositiveDecimal(); /* Initializes to 1 */
            virtual void setValue( DecimalType value );
            virtual void parse( const std::string& value );
        };
        
        /* MIN = 0, MAX = N/A, DEFAULT = 0 */
        class NonNegativeDecimal: public Decimal
        {
        public:
            explicit NonNegativeDecimal( DecimalType value );
            NonNegativeDecimal();
            virtual void setValue( DecimalType value );
            virtual void parse( const std::string& value );
        };
        
        /* MIN = 0, MAX = N/A, DEFAULT = 0 */
        using DivisionsValue = Decimal;
        using MillimetersValue = Decimal;
        
        /* MIN = 0, MAX = 100, DEFAULT = 0 */
        class Percent : public DecimalRange
        {
        public:
            explicit Percent( DecimalType value );
            Percent();
        };
        
        using PositiveDivisionsValue = PositiveDecimal;
        
        /* MIN = -180, MAX = 180, DEFAULT = 0 */
        class RotationDegrees : public DecimalRange
        {
        public:
            explicit RotationDegrees( DecimalType value );
            RotationDegrees();
        };
        
        using Semitones = Decimal;
        using TenthsValue = Decimal;
        
        /* MIN = 2, MAX = N/A, DEFAULT = 2 */
        class TrillBeats: public Decimal
        {
        public:
            explicit TrillBeats( DecimalType value );
            TrillBeats();
            // virtual ~TrillBeats();
            virtual void setValue( DecimalType value );
            virtual void parse( const std::string& value );
        };
    }
}
