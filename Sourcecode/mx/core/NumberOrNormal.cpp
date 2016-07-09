// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/NumberOrNormal.h"
#include <string>
#include <sstream>

namespace mx
{
    namespace core
    {
        
        class NumberOrNormal::impl
        {
        public:
            explicit impl()
            :myDecimal( 0 )
            ,myIsNormal( true )
            {}
            
            explicit impl( const Decimal& value )
            :myDecimal( value )
            ,myIsNormal( false )
            {}
            
            explicit impl( const std::string& value )
            :myDecimal( 0 )
            ,myIsNormal( false )
            {
                parse( value );
            }
            
            bool getIsNormal() const
            {
                return myIsNormal;
            }
            bool getIsNumber() const
            {
                return ! myIsNormal;
            }
            void setValueNormal()
            {
                myDecimal = Decimal( 0 );
                myIsNormal = true;;
            }
            void setValue( const Decimal& value )
            {
                myDecimal = Decimal( value );
                myIsNormal = false;
            }
            Decimal getValueNumber() const
            {
                return myDecimal;
            }
            void parse( const std::string& value )
            {
                if ( value == "normal" )
                {
                    myDecimal = Decimal( 0 );
                    myIsNormal = true;
                }
                else
                {
                    /* if it contains only numeric
                     characters it must be a number */
                    myDecimal.parse( value );
                    myIsNormal = false;
                }
            }
        private:
            Decimal myDecimal;
            bool myIsNormal;
        };
        
        
        NumberOrNormal::NumberOrNormal()
        :myImpl( new impl() )
        {}
        
        NumberOrNormal::NumberOrNormal( const Decimal& value )
        :myImpl( new impl( value ) )
        {}
        
        NumberOrNormal::NumberOrNormal( const std::string& value )
        :myImpl( new impl( value ) )
        {}
        
        NumberOrNormal::~NumberOrNormal() {}
        
        NumberOrNormal::NumberOrNormal( const NumberOrNormal& other )
        :myImpl( new NumberOrNormal::impl( *other.myImpl ) )
        {}
        
        NumberOrNormal::NumberOrNormal( NumberOrNormal&& other )
        :myImpl( std::move( other.myImpl ) )
        {}
        
        NumberOrNormal& NumberOrNormal::operator=( NumberOrNormal&& other )
        {
            myImpl = std::move( other.myImpl );
            return *this;
        }
        
        NumberOrNormal& NumberOrNormal::operator=( const NumberOrNormal& other )
        {
            this->myImpl = std::unique_ptr<NumberOrNormal::impl>( new NumberOrNormal::impl( *other.myImpl ) );
            return *this;
        }
        bool NumberOrNormal::getIsNormal() const
        {
            return myImpl->getIsNormal();
        }
        bool NumberOrNormal::getIsNumber() const
        {
            return myImpl->getIsNumber();
        }
        void NumberOrNormal::setValueNormal()
        {
            myImpl->setValueNormal();
        }
        void NumberOrNormal::setValue( const Decimal& value )
        {
            myImpl->setValue( value );
        }
        Decimal NumberOrNormal::getValueNumber() const
        {
            return myImpl->getValueNumber();
        }
        
        void NumberOrNormal::parse( const std::string& value )
        {
            myImpl->parse( value );
        }
        
        std::string toString( const NumberOrNormal& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
		std::ostream& toStream( std::ostream& os, const NumberOrNormal& value )
        {
            if ( value.getIsNumber() )
            {
                toStream( os, value.getValueNumber() );
            }
            else
            {
                os << "normal";
            }
            return os;
        }
		std::ostream& operator<<( std::ostream& os, const NumberOrNormal& value )
        {
            return toStream( os, value );
        }
        
        
    }
}
