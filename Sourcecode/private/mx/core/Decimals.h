// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <functional>
#include <iostream>
#include <string>

namespace mx
{
    namespace core
    {
        /// The type we use for non-integral numbers in this library.
        using DecimalType = long double;
        constexpr const DecimalType DecimalMin = std::numeric_limits<DecimalType>::min();
        constexpr const DecimalType DecimalMax = std::numeric_limits<DecimalType>::max();
        
        /// Decimals will be rounded to this number of places.
        constexpr const unsigned short DEFAULT_PRECISION = 6;
        
        /// When a Decimal's range is Exclusive and you try to assign a number
        /// that is out-of-range, this amount will be added to the exclusive
        /// minimum. For example if a Decimal is constrained to be a positive
        /// number, and you try to assign the value 0, the value will be set to
        /// NON_ZERO_AMOUNT instead.
        constexpr const DecimalType NON_ZERO_AMOUNT = 0.000001;

        using DecimalType = long double;

        /// @brief A precision representation of a decimal number
        ///
        /// @detailed Represents a decimal number with precision. Clamps
        /// if an attempt is made to assign an unrepresentable number.
        /// Rounds input values to the given MaxDecimalDigits or if a
        /// change in precision would lead to data loss.  Precision can
        /// be changed at runtime. A number such as 123.4567 would require
        /// a minimum precision setting of MaxIntegerDigits = 3,
        /// MaxDecimalDigits = 4.
        ///
        class PreciseDecimal
        {
        public:
            PreciseDecimal( short inMaxIntegerDigits, short inMaxDecimalDigits, DecimalType inValue );
            PreciseDecimal( short inMaxIntegerDigits, short inMaxDecimalDigits );
            PreciseDecimal( DecimalType inValue);
            PreciseDecimal();
            DecimalType getValue() const;
            void setValue( DecimalType inValue );
            short getMaxIntegerDigits() const;
            short getMaxDecimalDigits() const;
            DecimalType getMaxExpressibleNumber() const;
            DecimalType getMinExpressibleNumber() const;
            std::ostream& toStream( std::ostream& os ) const;
            std::string toString() const;

        private:
            bool myIsNegative;
            uint64_t myInteger;
            uint64_t myDecimal;
            short myMaxIntegerDigits;
            short myMaxDecimalDigits;
            uint64_t myMaxExpressibleInteger;
            uint64_t myMaxExpressibleDecimal;

        private:
            void setMaxIntegerDigits( short inNumDigits );
            void setMaxDecimalDigits( short inNumDigits );
            static decltype( myInteger ) findMax( short inNumDigits );
            void setMaxExpressibleInteger();
            void setMaxExpressibleDecimal();
            void setMaxExpressibleNumber();
            static DecimalType express( uint64_t inInteger, uint64_t inDecimal, short inMaxDecimalDigits, bool inIsNegative );

        private:
            static constexpr const short DEFAULT_MAX_INTEGER_DIGITS = 10;
            static constexpr const short DEFAULT_MAX_DECIMAL_DIGITS = 6;
            static constexpr const short MAX_STORAGE_TYPE_DIGITS = 19;
            static constexpr const uint64_t MAX_STORAGE_TYPE_VALUE = 9999999999999999999ULL;
        };

        class Decimal
        {
        public:
            Decimal();
            explicit Decimal( DecimalType value );
            virtual ~Decimal() = default;
            Decimal( const Decimal& ) = default;
            Decimal( Decimal&& ) = default;
            Decimal& operator=( const Decimal& ) = default;
            Decimal& operator=( Decimal&& ) = default;
            DecimalType getValue() const;
            virtual void setValue( DecimalType value );
            bool parse( const std::string& value );
            std::string toString() const;
            std::ostream& toStream( std::ostream& os ) const;
        private:
            PreciseDecimal myValue;
        };
        
        std::string toString( const Decimal& value, unsigned short precision = DEFAULT_PRECISION );
        std::ostream& toStream( std::ostream& os, const Decimal& value, unsigned short precision = DEFAULT_PRECISION );
        std::ostream& operator<<( std::ostream& os, const Decimal& value );

        /// This type is an implementation detail. It represent a function that
        /// clamps a DecimalType to a range.
        using DecimalClamp = std::function<DecimalType(DecimalType)>;

        /// A 'clamped', or 'ranged' Decimal where the value of min and max are
        /// governed by the DecimalClamp functions.
        class DecimalRange : public Decimal
        {
        public:
            explicit DecimalRange( DecimalClamp min, DecimalClamp max, DecimalType value );
            virtual void setValue( DecimalType value );
        private:
            DecimalClamp myMinClamp;
            DecimalClamp myMaxClamp;
        };

        /// The divisions type is used to express values in terms of the musical divisions defined
        /// by the divisions element. It is preferred that these be integer values both for MIDI
        /// interoperability and to avoid roundoff errors.
        ///
        /// Range: min=None, max=None
        class DivisionsValue : public DecimalRange
        {
        public:
            explicit DivisionsValue( DecimalType value );
            DivisionsValue();
        };

        /// The millimeters type is a number representing millimeters. This is used in the scaling
        /// element to provide a default scaling from tenths to physical units.
        ///
        /// Range: min=None, max=None
        class MillimetersValue : public DecimalRange
        {
        public:
            explicit MillimetersValue( DecimalType value );
            MillimetersValue();
        };

        /// The non-negative-decimal type specifies a non-negative decimal value.
        ///
        /// Range: min=Inclusive(0), max=None
        class NonNegativeDecimal : public DecimalRange
        {
        public:
            explicit NonNegativeDecimal( DecimalType value );
            NonNegativeDecimal();
        };

        /// The percent type specifies a percentage from 0 to 100.
        ///
        /// Range: min=Inclusive(0), max=Inclusive(100)
        class Percent : public DecimalRange
        {
        public:
            explicit Percent( DecimalType value );
            Percent();
        };

        /// The positive-decimal type specifies a positive decimal value.
        ///
        /// Range: min=Exclusive(0), max=None
        class PositiveDecimal : public DecimalRange
        {
        public:
            explicit PositiveDecimal( DecimalType value );
            PositiveDecimal();
        };

        /// The positive-divisions type restricts divisions values to positive numbers.
        ///
        /// Range: min=Exclusive(0), max=None
        class PositiveDivisions : public DecimalRange
        {
        public:
            explicit PositiveDivisions( DecimalType value );
            PositiveDivisions();
        };

        /// TODO - this needs to be properly generated
        ///
        /// Range: min=Exclusive(0), max=None
        class PositiveDivisionsValue : public DecimalRange
        {
        public:
            explicit PositiveDivisionsValue( DecimalType value );
            PositiveDivisionsValue();
        };

        /// The rotation-degrees type specifies rotation, pan, and elevation values in degrees.
        /// Values range from -180 to 180.
        ///
        /// Range: min=Inclusive(-180), max=Inclusive(180)
        class RotationDegrees : public DecimalRange
        {
        public:
            explicit RotationDegrees( DecimalType value );
            RotationDegrees();
        };

        /// The semitones type is a number representing semitones, used for chromatic alteration. A
        /// value of -1 corresponds to a flat and a value of 1 to a sharp. Decimal values like 0.5
        /// (quarter tone sharp) are used for microtones.
        ///
        /// Range: min=None, max=None
        class Semitones : public DecimalRange
        {
        public:
            explicit Semitones( DecimalType value );
            Semitones();
        };

        /// The tenths type is a number representing tenths of interline staff space (positive or
        /// negative). Both integer and decimal values are allowed, such as 5 for a half space and
        /// 2.5 for a quarter space. Interline space is measured from the middle of a staff line.
        ///
        /// Distances in a MusicXML file are measured in tenths of staff space. Tenths are then
        /// scaled to millimeters within the scaling element, used in the defaults element at the
        /// start of a score. Individual staves can apply a scaling factor to adjust staff size.
        /// When a MusicXML element or attribute refers to tenths, it means the global tenths
        /// defined by the scaling element, not the local tenths as adjusted by the staff-size
        /// element.
        ///
        /// Range: min=None, max=None
        class TenthsValue : public DecimalRange
        {
        public:
            explicit TenthsValue( DecimalType value );
            TenthsValue();
        };

        /// The trill-beats type specifies the beats used in a trill-sound or bend-sound attribute
        /// group. It is a decimal value with a minimum value of 2.
        ///
        /// Range: min=Inclusive(2), max=None
        class TrillBeats : public DecimalRange
        {
        public:
            explicit TrillBeats( DecimalType value );
            TrillBeats();
        };
    }
}
