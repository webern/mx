// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/FontSize.h"
#include <string>
#include <sstream>

namespace mx
{
    namespace core
    {

        class FontSize::impl
        {
        public:
            explicit impl()
            :myCssFontSize( CssFontSize::medium )
            ,myDecimal( 0 )
            ,myIsDecimal( false )
            {}
            
            explicit impl( const Decimal& value )
            :myCssFontSize( CssFontSize::medium )
            ,myDecimal( value )
            ,myIsDecimal( true )
            {}
            
            explicit impl( const CssFontSize value )
            :myCssFontSize( value )
            ,myDecimal( 0 )
            ,myIsDecimal( false )
            {}
            
            explicit impl( const std::string& value )
            :myCssFontSize( CssFontSize::medium )
            ,myDecimal( 0 )
            ,myIsDecimal( false )
            {
                parse( value );
            }
            
            bool getIsCssFontSize() const
            {
                return !myIsDecimal;
            }
            bool getIsNumber() const
            {
                return myIsDecimal;
            }
            void setValue( const CssFontSize value )
            {
                myCssFontSize = value;
                myIsDecimal = false;
            }
            void setValue( const Decimal& value )
            {
                myDecimal = Decimal( value );
                myIsDecimal = true;
            }
            CssFontSize getValueCssFontSize() const
            {
                return myCssFontSize;
            }
            Decimal getValueNumber() const
            {
                return myDecimal;
            }
            void parse( const std::string& value )
            {
                Decimal d( 0 );
                CssFontSize e = CssFontSize::medium;
                if ( value.find_first_not_of( "-.0123456789" ) != std::string::npos )
                {
                    if ( value.find_first_not_of( "-xsmalrgemdiu" ) == std::string::npos )
                    {
                        /* it must be an enum if it has
                         non numeric characters */
                        e = parseCssFontSize( value );
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
            CssFontSize myCssFontSize;
            Decimal myDecimal;
            bool myIsDecimal;
        };

        
        FontSize::FontSize()
        :myImpl( new impl() )
        {}
        
        FontSize::FontSize( const Decimal& value )
        :myImpl( new impl( value ) )
        {}
        
        FontSize::FontSize( const CssFontSize value )
        :myImpl( new impl( value ) )
        {}
        
        FontSize::FontSize( const std::string& value )
        :myImpl( new impl( value ) )
        {}
        
        FontSize::~FontSize() {}
        
        FontSize::FontSize( const FontSize& other )
        :myImpl( new FontSize::impl( *other.myImpl ) )
        {}
        
        FontSize::FontSize( FontSize&& other )
        :myImpl( std::move( other.myImpl ) )
        {}
        
        FontSize& FontSize::operator=( FontSize&& other )
        {
            myImpl = std::move( other.myImpl );
            return *this;
        }
        
        FontSize& FontSize::operator=( const FontSize& other )
        {
            this->myImpl = std::unique_ptr<FontSize::impl>( new FontSize::impl( *other.myImpl ) );
            return *this;
        }

        bool FontSize::getIsCssFontSize() const
        {
            return myImpl->getIsCssFontSize();
        }
        bool FontSize::getIsNumber() const
        {
            return myImpl->getIsNumber();
        }
        void FontSize::setValue( const CssFontSize value )
        {
            myImpl->setValue( value );
        }
        void FontSize::setValue( const Decimal& value )
        {
            myImpl->setValue( value );
        }

        CssFontSize FontSize::getValueCssFontSize() const
        {
            return myImpl->getValueCssFontSize();
        }
        Decimal FontSize::getValueNumber() const
        {
            return Decimal( myImpl->getValueNumber().getValue() );
        }
  
        void FontSize::parse( const std::string& value )
        {
            myImpl->parse( value );
        }

        std::string toString( const FontSize& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
		std::ostream& toStream( std::ostream& os, const FontSize& value )
        {
            if ( value.getIsNumber() )
            {
                toStream( os, value.getValueNumber() );
            }
            else
            {
                toStream( os, value.getValueCssFontSize() );
            }
            return os;
        }
		std::ostream& operator<<( std::ostream& os, const FontSize& value )
        {
            return toStream( os, value );
        }
        

    }
}
