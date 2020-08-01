// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <ostream>
#include <string>

namespace mx
{
    namespace core
    {
        /// Alias for the int type we will use throughout this library.
        using IntType = int;

        /// A base class for integer types
        class Integer
        {
        public:
            Integer();
            explicit Integer( IntType value );
            virtual ~Integer() = default;
            virtual IntType getValue() const final;
            virtual void setValue( IntType value );
            virtual void parse( const std::string& value ) final;
        protected:
            IntType myValue;
        };
        
        std::string toString( const Integer& value );
        std::ostream& toStream( std::ostream& os, const Integer& value );
        std::ostream& operator<<( std::ostream& os, const Integer& value );
        
        /// The accordion-middle type may have values of 1, 2, or 3, corresponding to
        /// having 1 to 3 dots in the middle section of the accordion registration symbol.
        /// Minimum: Inclusive(1), Maximum: Inclusive(3)
        /// The MusicXML format supports six levels of beaming, up to 1024th notes. Unlike
        /// the number-level type, the beam-level type identifies concurrent beams in a
        /// beam group. It does not distinguish overlapping beams such as grace notes
        /// within regular notes, or beams used in different voices.
        /// Minimum: Inclusive(1), Maximum: Inclusive(8)
        /// This is not part of MusicXML. It represents a clamped byte.
        /// Minimum: Inclusive(0), Maximum: Inclusive(255)
        /// The fifths type represents the number of flats or sharps in a traditional key
        /// signature. Negative numbers are used for flats and positive numbers for sharps,
        /// reflecting the key's placement within the circle of fifths (hence the type
        /// name).
        /// Minimum: Unbounded, Maximum: Unbounded
        /// The midi-16 type is used to express MIDI 1.0 values that range from 1 to 128.
        /// Minimum: Inclusive(1), Maximum: Inclusive(128)
        /// The midi-16 type is used to express MIDI 1.0 values that range from 1 to 16.
        /// Minimum: Inclusive(1), Maximum: Inclusive(16)
        /// The midi-16 type is used to express MIDI 1.0 values that range from 1 to
        /// 16,384.
        /// Minimum: Inclusive(1), Maximum: Inclusive(16384)
        /// The built-in primitive xs:nonNegativeInteger
        /// Minimum: Inclusive(0), Maximum: Unbounded
        /// Slurs, tuplets, and many other features can be concurrent and overlapping
        /// within a single musical part. The number-level type distinguishes up to six
        /// concurrent objects of the same type. A reading program should be prepared to
        /// handle cases where the number-levels stop in an arbitrary order. Different
        /// numbers are needed when the features overlap in MusicXML document order. When a
        /// number-level value is implied, the value is 1 by default.
        /// Minimum: Inclusive(1), Maximum: Inclusive(6)
        /// The number-of-lines type is used to specify the number of lines in text
        /// decoration attributes.
        /// Minimum: Inclusive(0), Maximum: Inclusive(3)
        /// Octaves are represented by the numbers 0 to 9, where 4 indicates the octave
        /// started by middle C.
        /// Minimum: Inclusive(0), Maximum: Inclusive(9)
        /// The built-in primitive xs:positiveInteger
        /// Minimum: Inclusive(1), Maximum: Unbounded
        /// The staff-line type indicates the line on a given staff. Staff lines are
        /// numbered from bottom to top, with 1 being the bottom line on a staff. Staff
        /// line values can be used to specify positions outside the staff, such as a C
        /// clef positioned in the middle of a grand staff.
        /// Minimum: Unbounded, Maximum: Unbounded
        /// The staff-number type indicates staff numbers within a multi-staff part. Staves
        /// are numbered from top to bottom, with 1 being the top staff on a part.
        /// Minimum: Inclusive(1), Maximum: Unbounded
        /// The string-number type indicates a string number. Strings are numbered from
        /// high to low, with 1 being the highest pitched string.
        /// Minimum: Inclusive(1), Maximum: Unbounded
        /// The number of tremolo marks is represented by a number from 0 to 8: the same as
        /// beam-level with 0 added.
        /// Minimum: Inclusive(0), Maximum: Inclusive(8)    }
}
