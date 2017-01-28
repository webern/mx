// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/FrameNote.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Barre.h"
#include "mx/core/elements/Fingering.h"
#include "mx/core/elements/Fret.h"
#include "mx/core/elements/String.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FrameNote::FrameNote()
        :myString( makeString() )
        ,myFret( makeFret() )
        ,myFingering( makeFingering() )
        ,myHasFingering( false )
        ,myBarre( makeBarre() )
        ,myHasBarre( false )
        {}


        bool FrameNote::hasAttributes() const
        {
            return false;
        }


        std::ostream& FrameNote::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& FrameNote::streamName( std::ostream& os ) const
        {
            os << "frame-note";
            return os;
        }


        bool FrameNote::hasContents() const
        {
            return true;
        }


        std::ostream& FrameNote::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myString->toStream( os, indentLevel+1 );
            os << std::endl;
            myFret->toStream( os, indentLevel+1 );
            if ( myHasFingering )
            {
                os << std::endl;
                myFingering->toStream( os, indentLevel+1 );
            }
            if ( myHasBarre )
            {
                os << std::endl;
                myBarre->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        StringPtr FrameNote::getString() const
        {
            return myString;
        }


        void FrameNote::setString( const StringPtr& value )
        {
            if( value )
            {
                myString = value;
            }
        }


        FretPtr FrameNote::getFret() const
        {
            return myFret;
        }


        void FrameNote::setFret( const FretPtr& value )
        {
            if( value )
            {
                myFret = value;
            }
        }


        FingeringPtr FrameNote::getFingering() const
        {
            return myFingering;
        }


        void FrameNote::setFingering( const FingeringPtr& value )
        {
            if( value )
            {
                myFingering = value;
            }
        }


        bool FrameNote::getHasFingering() const
        {
            return myHasFingering;
        }


        void FrameNote::setHasFingering( const bool value )
        {
            myHasFingering = value;
        }


        BarrePtr FrameNote::getBarre() const
        {
            return myBarre;
        }


        void FrameNote::setBarre( const BarrePtr& value )
        {
            if( value )
            {
                myBarre = value;
            }
        }


        bool FrameNote::getHasBarre() const
        {
            return myHasBarre;
        }


        void FrameNote::setHasBarre( const bool value )
        {
            myHasBarre = value;
        }


        bool FrameNote::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isStringFound = false;
            bool isFretFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myString, isStringFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myFret, isFretFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myFingering, myHasFingering ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myBarre, myHasBarre ) ) { continue; }
            }

            if( !isStringFound )
            {
                message << "FrameNote: '" << myString->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isFretFound )
            {
                message << "FrameNote: '" << myFret->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
