// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/CommaSeparatedText.h"

#include <set>

namespace mx
{
    namespace core
    {
        class CommaSeparatedListOfPositiveIntegers
        {
        public:
            CommaSeparatedListOfPositiveIntegers();
            CommaSeparatedListOfPositiveIntegers( const StringType& value );
            CommaSeparatedListOfPositiveIntegers( const std::set<int>& values );
            const std::set<int>& getValues() const;
            std::set<int>::iterator getValuesBegin();
            std::set<int>::iterator getValuesEnd();
            std::set<int>::const_iterator getValuesBeginConst() const;
            std::set<int>::const_iterator getValuesEndConst() const;
            void setValues( const std::set<int>& values );
            void parse( const StringType& commaSeparatedText );
            void setUseSpaceBetweenItems( bool value );
            bool getUseSpaceBetweenItems() const;
        private:
            std::set<int> myValues;
            bool myIsSpacingDesired;
        };
        
        StringType toString( const CommaSeparatedListOfPositiveIntegers& value );
        std::ostream& toStream( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value );
        std::ostream& operator<<( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value );
    }
}
