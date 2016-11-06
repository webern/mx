// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/Color.h"
#include <sstream>
#include <iomanip>

namespace mx
{
    namespace core
    {
        class Color::impl
        {
        public:
            impl( const IntType alpha,
                 const IntType red,
                 const IntType green,
                 const IntType blue )
            :myAlpha( 0, 255, alpha )
            ,myRed( 0, 255, red )
            ,myGreen( 0, 255, green )
            ,myBlue( 0, 255, blue )
            ,myColorType( Color::ColorType::ARGB )
            {}
            
            impl( const IntType red,
                  const IntType green,
                  const IntType blue )
            :myAlpha( 0, 255, 255 )
            ,myRed( 0, 255, red )
            ,myGreen( 0, 255, green )
            ,myBlue( 0, 255, blue )
            ,myColorType( Color::ColorType::RGB )
            {}
            
            impl()
            :myAlpha( 0, 255, 255 )
            ,myRed( 0, 255, 255 )
            ,myGreen( 0, 255, 255 )
            ,myBlue( 0, 255, 255 )
            ,myColorType( ColorType::RGB )
            {}
            
            IntType getAlpha() const
            {
                return (short)myAlpha.getValue();
            }
            IntType getRed() const
            {
                return (IntType)myRed.getValue();
            }
            IntType getGreen() const
            {
                return (IntType)myGreen.getValue();
            }
            IntType getBlue() const
            {
                return (IntType)myBlue.getValue();
            }
            ColorType getColorType() const
            {
                return myColorType;
            }
            
            void setAlpha( const IntType value )
            {
                myAlpha.setValue( value );
                myColorType = Color::ColorType::ARGB;
            }
            void setRed( const IntType value )
            {
                myRed.setValue( value );
            }
            void setGreen( const IntType value )
            {
                myGreen.setValue( value );
            }
            void setBlue( const IntType value )
            {
                myBlue.setValue( value );
            }
            void setColorType( const ColorType value )
            {
                myColorType = value;
                if ( myColorType == Color::ColorType::RGB )
                {
                    myAlpha.setValue( 255 );
                }
            }
            void parse( const std::string& value )
            {
                if ( value.length() != 7 &&
                     value.length() != 9 )
                {
                    return; // bad input
                }
                auto c = value.cbegin();
                if ( *c != '#' )
                {
                    return; // bad input
                }
                if ( value.substr( 1, value.length()-1).find_first_not_of( "ABCDEF0123456789" )
                    != std::string::npos )
                {
                    return; // bad input
                }
                Color::ColorType colortype = Color::ColorType::RGB;
                IntType alpha = 255;
                IntType red = 255;
                IntType green = 255;
                IntType blue = 255;
                if ( value.length() == 9 )
                {
                    colortype = Color::ColorType::ARGB;
                    alpha = getInt( value, 1 );
                    red = getInt( value, 3 );
                    green = getInt( value, 5 );
                    blue = getInt( value, 7 );
                }
                else if ( value.length() == 7 )
                {
                    colortype = Color::ColorType::RGB;
                    alpha = 255;
                    red = getInt( value, 1 );
                    green = getInt( value, 3 );
                    blue = getInt( value, 5 );
                }
                setAlpha( alpha );
                setRed( red );
                setGreen( green );
                setBlue( blue );
                setColorType( colortype );
            }
            
        private:
            IntRange myAlpha;
            IntRange myRed;
            IntRange myGreen;
            IntRange myBlue;
            Color::ColorType myColorType;
            IntType getInt( const std::string& str, int startpos )
            {
                IntType val = 0;
                std::stringstream ss( str.substr( static_cast<size_t>( startpos ), 2 ) );
                ss << std::hex;
                ss << std::uppercase;
                ss >> val;
                return val;
            }
        };
        
        
        Color::Color( const IntType alpha,
                      const IntType red,
                      const IntType green,
                      const IntType blue )
        :myImpl( new Color::impl( alpha, red, green, blue ) )
        {}
        
        Color::Color( const IntType red,
                      const IntType green,
                      const IntType blue )
        :myImpl( new Color::impl( red, green, blue ) )
        {}
        
        Color::Color()
        :myImpl( new Color::impl() )
        {}
        
        Color::Color( const std::string& value )
        :myImpl( new Color::impl() )
        { myImpl->parse( value ); }

        Color::~Color() {}
        
        Color::Color( const Color& other )
        :myImpl( new Color::impl( *other.myImpl ) )
        {}
        
        Color::Color( Color&& other )
        :myImpl( std::move( other.myImpl ) )
        {}
        
        Color& Color::operator=( const Color& other )
        {
            this->myImpl = std::unique_ptr<Color::impl>( new Color::impl( *other.myImpl ) );
            return *this;
        }
        
        IntType Color::getAlpha() const
        {
            return myImpl->getAlpha();
        }
        IntType Color::getRed() const
        {
            return myImpl->getRed();
        }
        IntType Color::getGreen() const
        {
            return myImpl->getGreen();
        }
        IntType Color::getBlue() const
        {
            return myImpl->getBlue();
        }
        Color::ColorType Color::getColorType() const
        {
            return myImpl->getColorType();
        }
        void Color::setAlpha( const IntType value )
        {
            myImpl->setAlpha( value );
        }
        void Color::setRed( const IntType value )
        {
            myImpl->setRed( value );
        }
        void Color::setGreen( const IntType value )
        {
            myImpl->setGreen( value );
        }
        void Color::setBlue( const IntType value )
        {
            myImpl->setBlue( value );
        }
        void Color::setColorType( const Color::ColorType value )
        {
            myImpl->setColorType( value );
        }
        void Color::parse( const std::string& value )
        {
            myImpl->parse( value );
        }
 
        std::string toString( const Color& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
		std::ostream& toStream( std::ostream& os, const Color& value )
        {
            auto flagsval = os.flags();
            auto fillval = os.fill();
            auto widthval = os.width();
            os << "#";
            
            if ( value.getColorType() == Color::ColorType::ARGB )
            {
                os << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << value.getAlpha();
            }
            os  << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << value.getRed();
            os  << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << value.getGreen();
            os  << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << value.getBlue();
            os.flags( flagsval );
            os.width( widthval );
            os.flags( flagsval );
            os.fill( fillval );
            return os;
        }
		std::ostream& operator<<( std::ostream& os, const Color& value )
        {
            return toStream( os, value );
        }
    }
}
