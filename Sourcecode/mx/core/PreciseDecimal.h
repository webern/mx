// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <cstdint>
#include <iostream>

namespace mx
{
    namespace core
    {
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

        // 18,446,744,073,709,551,615
    }
}
