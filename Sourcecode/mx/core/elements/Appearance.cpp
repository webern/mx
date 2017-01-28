// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Appearance.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Distance.h"
#include "mx/core/elements/LineWidth.h"
#include "mx/core/elements/NoteSize.h"
#include "mx/core/elements/OtherAppearance.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Appearance::Appearance()
        :myLineWidthSet()
        ,myNoteSizeSet()
        ,myDistanceSet()
        ,myOtherAppearanceSet()
        {}


        bool Appearance::hasAttributes() const
        {
            return false;
        }


        std::ostream& Appearance::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Appearance::streamName( std::ostream& os ) const
        {
            os << "appearance";
            return os;
        }


        bool Appearance::hasContents() const
        {
            return myLineWidthSet.size() > 0
            || myNoteSizeSet.size() > 0
            || myDistanceSet.size() > 0
            || myOtherAppearanceSet.size() > 0;
        }


        std::ostream& Appearance::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto x : myLineWidthSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myNoteSizeSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myDistanceSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myOtherAppearanceSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        const LineWidthSet& Appearance::getLineWidthSet() const
        {
            return myLineWidthSet;
        }


        void Appearance::removeLineWidth( const LineWidthSetIterConst& value )
        {
            if ( value != myLineWidthSet.cend() )
            {
                myLineWidthSet.erase( value );
            }
        }


        void Appearance::addLineWidth( const LineWidthPtr& value )
        {
            if ( value )
            {
                myLineWidthSet.push_back( value );
            }
        }


        void Appearance::clearLineWidthSet()
        {
            myLineWidthSet.clear();
        }


        LineWidthPtr Appearance::getLineWidth( const LineWidthSetIterConst& setIterator ) const
        {
            if( setIterator != myLineWidthSet.cend() )
            {
                return *setIterator;
            }
            return LineWidthPtr();
        }


        const NoteSizeSet& Appearance::getNoteSizeSet() const
        {
            return myNoteSizeSet;
        }


        void Appearance::removeNoteSize( const NoteSizeSetIterConst& value )
        {
            if ( value != myNoteSizeSet.cend() )
            {
                myNoteSizeSet.erase( value );
            }
        }


        void Appearance::addNoteSize( const NoteSizePtr& value )
        {
            if ( value )
            {
                myNoteSizeSet.push_back( value );
            }
        }


        void Appearance::clearNoteSizeSet()
        {
            myNoteSizeSet.clear();
        }


        NoteSizePtr Appearance::getNoteSize( const NoteSizeSetIterConst& setIterator ) const
        {
            if( setIterator != myNoteSizeSet.cend() )
            {
                return *setIterator;
            }
            return NoteSizePtr();
        }


        const DistanceSet& Appearance::getDistanceSet() const
        {
            return myDistanceSet;
        }


        void Appearance::removeDistance( const DistanceSetIterConst& value )
        {
            if ( value != myDistanceSet.cend() )
            {
                myDistanceSet.erase( value );
            }
        }


        void Appearance::addDistance( const DistancePtr& value )
        {
            if ( value )
            {
                myDistanceSet.push_back( value );
            }
        }


        void Appearance::clearDistanceSet()
        {
            myDistanceSet.clear();
        }


        DistancePtr Appearance::getDistance( const DistanceSetIterConst& setIterator ) const
        {
            if( setIterator != myDistanceSet.cend() )
            {
                return *setIterator;
            }
            return DistancePtr();
        }


        const OtherAppearanceSet& Appearance::getOtherAppearanceSet() const
        {
            return myOtherAppearanceSet;
        }


        void Appearance::removeOtherAppearance( const OtherAppearanceSetIterConst& value )
        {
            if ( value != myOtherAppearanceSet.cend() )
            {
                myOtherAppearanceSet.erase( value );
            }
        }


        void Appearance::addOtherAppearance( const OtherAppearancePtr& value )
        {
            if ( value )
            {
                myOtherAppearanceSet.push_back( value );
            }
        }


        void Appearance::clearOtherAppearanceSet()
        {
            myOtherAppearanceSet.clear();
        }


        OtherAppearancePtr Appearance::getOtherAppearance( const OtherAppearanceSetIterConst& setIterator ) const
        {
            if( setIterator != myOtherAppearanceSet.cend() )
            {
                return *setIterator;
            }
            return OtherAppearancePtr();
        }


        bool Appearance::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "line-width", myLineWidthSet );
                importElementSet( message, it, endIter, isSuccess, "note-size", myNoteSizeSet );
                importElementSet( message, it, endIter, isSuccess, "distance", myDistanceSet );
                importElementSet( message, it, endIter, isSuccess, "other-appearance", myOtherAppearanceSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
