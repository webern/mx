// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Frame.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/FirstFret.h"
#include "mx/core/elements/FrameFrets.h"
#include "mx/core/elements/FrameNote.h"
#include "mx/core/elements/FrameStrings.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Frame::Frame()
        :myAttributes( std::make_shared<FrameAttributes>() )
        ,myFrameStrings( makeFrameStrings() )
        ,myFrameFrets( makeFrameFrets() )
        ,myFirstFret( makeFirstFret() )
        ,myHasFirstFret( false )
        ,myFrameNoteSet()
        {
            myFrameNoteSet.push_back( makeFrameNote() );
        }


        bool Frame::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Frame::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Frame::streamName( std::ostream& os ) const
        {
            os << "frame";
            return os;
        }


        bool Frame::hasContents() const
        {
            return true;
        }


        std::ostream& Frame::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myFrameStrings->toStream( os, indentLevel+1 );
            os << std::endl;
            myFrameFrets->toStream( os, indentLevel+1 );
            if ( myHasFirstFret )
            {
                os << std::endl;
                myFirstFret->toStream( os, indentLevel+1 );
            }
            for ( auto x : myFrameNoteSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        FrameAttributesPtr Frame::getAttributes() const
        {
            return myAttributes;
        }


        void Frame::setAttributes( const FrameAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        FrameStringsPtr Frame::getFrameStrings() const
        {
            return myFrameStrings;
        }


        void Frame::setFrameStrings( const FrameStringsPtr& value )
        {
            if( value )
            {
                myFrameStrings = value;
            }
        }


        FrameFretsPtr Frame::getFrameFrets() const
        {
            return myFrameFrets;
        }


        void Frame::setFrameFrets( const FrameFretsPtr& value )
        {
            if( value )
            {
                myFrameFrets = value;
            }
        }


        FirstFretPtr Frame::getFirstFret() const
        {
            return myFirstFret;
        }


        void Frame::setFirstFret( const FirstFretPtr& value )
        {
            if( value )
            {
                myFirstFret = value;
            }
        }


        bool Frame::getHasFirstFret() const
        {
            return myHasFirstFret;
        }


        void Frame::setHasFirstFret( const bool value )
        {
            myHasFirstFret = value;
        }


        const FrameNoteSet& Frame::getFrameNoteSet() const
        {
            return myFrameNoteSet;
        }


        void Frame::removeFrameNote( const FrameNoteSetIterConst& value )
        {
            if ( value != myFrameNoteSet.cend() )
            {
                if ( myFrameNoteSet.size() > 1 )
                {
                    myFrameNoteSet.erase( value );
                }
            }
        }


        void Frame::addFrameNote( const FrameNotePtr& value )
        {
            if ( value )
            {
                myFrameNoteSet.push_back( value );
            }
        }


        void Frame::clearFrameNoteSet()
        {
            myFrameNoteSet.clear();
            myFrameNoteSet.push_back( makeFrameNote() );
        }


        FrameNotePtr Frame::getFrameNote( const FrameNoteSetIterConst& setIterator ) const
        {
            if( setIterator != myFrameNoteSet.cend() )
            {
                return *setIterator;
            }
            return FrameNotePtr();
        }


        bool Frame::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isFrameStringsFound = false;
            bool isFrameFretsFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myFrameStrings, isFrameStringsFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myFrameFrets, isFrameFretsFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myFirstFret, myHasFirstFret ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "frame-note", myFrameNoteSet );
            }

            if( !isFrameStringsFound )
            {
                message << "Frame: '" << myFrameStrings->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isFrameFretsFound )
            {
                message << "Frame: '" << myFrameFrets->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
