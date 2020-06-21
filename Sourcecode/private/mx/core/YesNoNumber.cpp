// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/YesNoNumber.h"
#include <sstream>

namespace mx
{
    namespace core
    {
        
        class YesNoNumber::impl
        {
        public:
            explicit impl()
            :myYesNo( YesNo::no )
            ,myDecimal( 0 )
            ,myIsDecimal( false )
            {}
            
            explicit impl( const Decimal& value )
            :myYesNo( YesNo::no )
            ,myDecimal( value )
            ,myIsDecimal( true )
            {}
            
            explicit impl( const YesNo value )
            :myYesNo( value )
            ,myDecimal( 0 )
            ,myIsDecimal( false )
            {}
            
            explicit impl( const std::string& value )
            :myYesNo( YesNo::no )
            ,myDecimal( 0 )
            ,myIsDecimal( false )
            {
                parse( value );
            }
            
            bool getIsYesNo() const
            {
                return !myIsDecimal;
            }
            bool getIsNumber() const
            {
                return myIsDecimal;
            }
            void setValue( const YesNo value )
            {
                myYesNo = value;
                myIsDecimal = false;
            }
            void setValue( const Decimal& value )
            {
                myDecimal = Decimal( value );
                myIsDecimal = true;
            }
            YesNo getValueYesNo() const
            {
                return myYesNo;
            }
            Decimal getValueNumber() const
            {
                return myDecimal;
            }
            void parse( const std::string& value )
            {
                Decimal d( 0 );
                YesNo e = YesNo::no;
                if ( value.find_first_not_of( "-.0123456789" ) != std::string::npos )
                {
                    if ( value.find_first_not_of( "yesno" ) == std::string::npos )
                    {
                        /* it must be an enum if it has
                         non numeric characters */
                        e = parseYesNo( value );
                        this->setValue( e );
                    }
                }
                else
                {
                    /* if it contains only numeric
                     characters it must be a number */
                    d.parse( value );
                    this->setValue( d );
                }
            }
            
        private:
            YesNo myYesNo;
            Decimal myDecimal;
            bool myIsDecimal;
        };
        
        
        YesNoNumber::YesNoNumber()
        :myImpl( new impl() )
        {}
        
        YesNoNumber::YesNoNumber( const Decimal& value )
        :myImpl( new impl( value ) )
        {}
        
        YesNoNumber::YesNoNumber( const YesNo value )
        :myImpl( new impl( value ) )
        {}
        
        YesNoNumber::YesNoNumber( const std::string& value )
        :myImpl( new impl( value ) )
        {}
        
        YesNoNumber::~YesNoNumber() {}
        
        YesNoNumber::YesNoNumber( const YesNoNumber& other )
        :myImpl( new YesNoNumber::impl( *other.myImpl ) )
        {}
        
        YesNoNumber::YesNoNumber( YesNoNumber&& other )
        :myImpl( std::move( other.myImpl ) )
        {}
        
        YesNoNumber& YesNoNumber::operator=( YesNoNumber&& other )
        {
            myImpl = std::move( other.myImpl );
            return *this;
        }
        
        YesNoNumber& YesNoNumber::operator=( const YesNoNumber& other )
        {
            this->myImpl = std::unique_ptr<YesNoNumber::impl>( new YesNoNumber::impl( *other.myImpl ) );
            return *this;
        }
        
        bool YesNoNumber::getIsYesNo() const
        {
            return myImpl->getIsYesNo();
        }
        bool YesNoNumber::getIsNumber() const
        {
            return myImpl->getIsNumber();
        }
        void YesNoNumber::setValue( const YesNo value )
        {
            myImpl->setValue( value );
        }
        void YesNoNumber::setValue( const Decimal& value )
        {
            myImpl->setValue( value );
        }
        
        YesNo YesNoNumber::getValueYesNo() const
        {
            return myImpl->getValueYesNo();
        }
        Decimal YesNoNumber::getValueNumber() const
        {
            return Decimal( myImpl->getValueNumber().getValue() );
        }
        
        void YesNoNumber::parse( const std::string& value )
        {
            myImpl->parse( value );
        }
        
        std::string toString( const YesNoNumber& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
		std::ostream& toStream( std::ostream& os, const YesNoNumber& value )
        {
            if ( value.getIsNumber() )
            {
                toStream( os, value.getValueNumber() );
            }
            else
            {
                toStream( os, value.getValueYesNo() );
            }
            return os;
        }
		std::ostream& operator<<( std::ostream& os, const YesNoNumber& value )
        {
            return toStream( os, value );
        }
        
        
    }
}
