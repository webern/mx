// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Clef.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ClefOctaveChange.h"
#include "mx/core/elements/Line.h"
#include "mx/core/elements/Sign.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Clef::Clef()
        :myAttributes( std::make_shared<ClefAttributes>() )
        ,mySign( makeSign() )
        ,myLine( makeLine() )
        ,myHasLine( false )
        ,myClefOctaveChange( makeClefOctaveChange() )
        ,myHasClefOctaveChange( false )
        {}


        bool Clef::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Clef::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Clef::streamName( std::ostream& os ) const
        {
            os << "clef";
            return os;
        }


        bool Clef::hasContents() const
        {
            return true;
        }


        std::ostream& Clef::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            mySign->toStream( os, indentLevel+1 );
            if ( myHasLine )
            {
                os << std::endl;
                myLine->toStream( os, indentLevel+1 );
            }
            if ( myHasClefOctaveChange )
            {
                os << std::endl;
                myClefOctaveChange->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        ClefAttributesPtr Clef::getAttributes() const
        {
            return myAttributes;
        }


        void Clef::setAttributes( const ClefAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        SignPtr Clef::getSign() const
        {
            return mySign;
        }


        void Clef::setSign( const SignPtr& value )
        {
            if( value )
            {
                mySign = value;
            }
        }


        LinePtr Clef::getLine() const
        {
            return myLine;
        }


        void Clef::setLine( const LinePtr& value )
        {
            if( value )
            {
                myLine = value;
            }
        }


        bool Clef::getHasLine() const
        {
            return myHasLine;
        }


        void Clef::setHasLine( const bool value )
        {
            myHasLine = value;
        }


        ClefOctaveChangePtr Clef::getClefOctaveChange() const
        {
            return myClefOctaveChange;
        }


        void Clef::setClefOctaveChange( const ClefOctaveChangePtr& value )
        {
            if( value )
            {
                myClefOctaveChange = value;
            }
        }


        bool Clef::getHasClefOctaveChange() const
        {
            return myHasClefOctaveChange;
        }


        void Clef::setHasClefOctaveChange( const bool value )
        {
            myHasClefOctaveChange = value;
        }


        bool Clef::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isSignFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *mySign, isSignFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myLine, myHasLine ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myClefOctaveChange, myHasClefOctaveChange ) ) { continue; }
            }

            if( !isSignFound )
            {
                message << "Clef: '" << mySign->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
