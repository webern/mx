// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include "mx/core/Integers.h"
#include <memory>

namespace mx
{
    namespace core
    {
        class PositiveIntegerOrEmpty
        {
        public:
            explicit PositiveIntegerOrEmpty();
            explicit PositiveIntegerOrEmpty( const PositiveInteger& value );
            explicit PositiveIntegerOrEmpty( const std::string& value );
            virtual ~PositiveIntegerOrEmpty();
            PositiveIntegerOrEmpty( const PositiveIntegerOrEmpty& other );
            PositiveIntegerOrEmpty( PositiveIntegerOrEmpty&& other );
            PositiveIntegerOrEmpty& operator=( const PositiveIntegerOrEmpty& other );
            PositiveIntegerOrEmpty& operator=( PositiveIntegerOrEmpty&& other );
            
            bool getIsEmpty() const;
            bool getIsNumber() const;
            void setValueEmpty();
            void setValue( const PositiveInteger& value );
            PositiveInteger getValueNumber() const;
            void parse( const std::string& value );
            
        private:
            class impl;
            std::unique_ptr<impl> myImpl;
        };
        
        std::string toString( const PositiveIntegerOrEmpty& value );
		std::ostream& toStream( std::ostream& os, const PositiveIntegerOrEmpty& value );
		std::ostream& operator<<( std::ostream& os, const PositiveIntegerOrEmpty& value );
    }
}
