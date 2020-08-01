// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <limits>
#include <string>

namespace mx
{
    namespace core
    {
        /// Alias for the int type used by this library.
        using IntType = int;
        constexpr const IntType IntMax = std::numeric_limits<IntType>::max();
        constexpr const IntType IntMin = std::numeric_limits<IntType>::min();

        /// A base class for all integer types.
        class Integer
        {
        public:
            Integer();
            explicit Integer( IntType value );
            virtual ~Integer() = default;
            IntType getValue() const;
            virtual void setValue( IntType value );
            virtual void parse( const std::string& value ) final;
        private:
            IntType myValue;
        };


        std::string toString( const Integer& value );
        std::ostream& toStream( std::ostream& os, const Integer& value );
        std::ostream& operator<<( std::ostream& os, const Integer& value );

        /// A 'clamped', or 'ranged' Integer where the value can never be set
        /// less than min or greater than max.
        class IntRange : public Integer
        {
        public:
            explicit IntRange( IntType min, IntType max, IntType value );
            virtual ~IntRange() = default;
            virtual void setValue( IntType value ) override;
        private:
            const IntType myMin;
            const IntType myMax;
        };

        /// The accordion-middle type may have values of 1, 2, or 3, corresponding to
        /// having 1 to 3 dots in the middle section of the accordion registration symbol.
        class AccordionMiddleValue : public IntRange
        {
        public:
            explicit AccordionMiddleValue( IntType value );
            AccordionMiddleValue();
        };

        /// The MusicXML format supports six levels of beaming, up to 1024th notes. Unlike
        /// the number-level type, the beam-level type identifies concurrent beams in a
        /// beam group. It does not distinguish overlapping beams such as grace notes
        /// within regular notes, or beams used in different voices.
        class BeamLevel : public IntRange
        {
        public:
            explicit BeamLevel( IntType value );
            BeamLevel();
        };

        /// This is not part of MusicXML. It represents a clamped byte.
        class Byte : public IntRange
        {
        public:
            explicit Byte( IntType value );
            Byte();
        };

        /// The fifths type represents the number of flats or sharps in a traditional key
        /// signature. Negative numbers are used for flats and positive numbers for sharps,
        /// reflecting the key's placement within the circle of fifths (hence the type
        /// name).
        class FifthsValue : public IntRange
        {
        public:
            explicit FifthsValue( IntType value );
            FifthsValue();
        };

        /// The midi-16 type is used to express MIDI 1.0 values that range from 1 to 128.
        class Midi128 : public IntRange
        {
        public:
            explicit Midi128( IntType value );
            Midi128();
        };

        /// The midi-16 type is used to express MIDI 1.0 values that range from 1 to 16.
        class Midi16 : public IntRange
        {
        public:
            explicit Midi16( IntType value );
            Midi16();
        };

        /// The midi-16 type is used to express MIDI 1.0 values that range from 1 to
        /// 16,384.
        class Midi16384 : public IntRange
        {
        public:
            explicit Midi16384( IntType value );
            Midi16384();
        };

        /// The built-in primitive xs:nonNegativeInteger
        class NonNegativeInteger : public IntRange
        {
        public:
            explicit NonNegativeInteger( IntType value );
            NonNegativeInteger();
        };

        /// Slurs, tuplets, and many other features can be concurrent and overlapping
        /// within a single musical part. The number-level type distinguishes up to six
        /// concurrent objects of the same type. A reading program should be prepared to
        /// handle cases where the number-levels stop in an arbitrary order. Different
        /// numbers are needed when the features overlap in MusicXML document order. When a
        /// number-level value is implied, the value is 1 by default.
        class NumberLevel : public IntRange
        {
        public:
            explicit NumberLevel( IntType value );
            NumberLevel();
        };

        /// The number-of-lines type is used to specify the number of lines in text
        /// decoration attributes.
        class NumberOfLines : public IntRange
        {
        public:
            explicit NumberOfLines( IntType value );
            NumberOfLines();
        };

        /// Octaves are represented by the numbers 0 to 9, where 4 indicates the octave
        /// started by middle C.
        class OctaveValue : public IntRange
        {
        public:
            explicit OctaveValue( IntType value );
            OctaveValue();
        };

        /// The built-in primitive xs:positiveInteger
        class PositiveInteger : public IntRange
        {
        public:
            explicit PositiveInteger( IntType value );
            PositiveInteger();
        };

        /// The staff-line type indicates the line on a given staff. Staff lines are
        /// numbered from bottom to top, with 1 being the bottom line on a staff. Staff
        /// line values can be used to specify positions outside the staff, such as a C
        /// clef positioned in the middle of a grand staff.
        class StaffLine : public IntRange
        {
        public:
            explicit StaffLine( IntType value );
            StaffLine();
        };

        /// The staff-number type indicates staff numbers within a multi-staff part. Staves
        /// are numbered from top to bottom, with 1 being the top staff on a part.
        class StaffNumber : public IntRange
        {
        public:
            explicit StaffNumber( IntType value );
            StaffNumber();
        };

        /// The string-number type indicates a string number. Strings are numbered from
        /// high to low, with 1 being the highest pitched string.
        class StringNumber : public IntRange
        {
        public:
            explicit StringNumber( IntType value );
            StringNumber();
        };

        /// The number of tremolo marks is represented by a number from 0 to 8: the same as
        /// beam-level with 0 added.
        class TremoloMarks : public IntRange
        {
        public:
            explicit TremoloMarks( IntType value );
            TremoloMarks();
        };
    }
}