// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MetronomeTuplet.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ActualNotes.h"
#include "mx/core/elements/NormalNotes.h"
#include "mx/core/elements/TimeModificationNormalTypeNormalDot.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeTuplet::MetronomeTuplet()
        :myAttributes( std::make_shared<MetronomeTupletAttributes>() )
        ,myActualNotes( makeActualNotes() )
        ,myNormalNotes( makeNormalNotes() )
        ,myTimeModificationNormalTypeNormalDot( makeTimeModificationNormalTypeNormalDot() )
        ,myHasTimeModificationNormalTypeNormalDot( false )
        {}


        bool MetronomeTuplet::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& MetronomeTuplet::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& MetronomeTuplet::streamName( std::ostream& os ) const
        {
            os << "metronome-tuplet";
            return os;
        }


        bool MetronomeTuplet::hasContents() const
        {
            return true;
        }


        std::ostream& MetronomeTuplet::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myActualNotes->toStream( os, indentLevel+1 );
            os << std::endl;
            myNormalNotes->toStream( os, indentLevel+1 );
            if ( myHasTimeModificationNormalTypeNormalDot )
            {
                os << std::endl;
                myTimeModificationNormalTypeNormalDot->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        MetronomeTupletAttributesPtr MetronomeTuplet::getAttributes() const
        {
            return myAttributes;
        }


        void MetronomeTuplet::setAttributes( const MetronomeTupletAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        ActualNotesPtr MetronomeTuplet::getActualNotes() const
        {
            return myActualNotes;
        }


        void MetronomeTuplet::setActualNotes( const ActualNotesPtr& value )
        {
            if( value )
            {
                myActualNotes = value;
            }
        }


        NormalNotesPtr MetronomeTuplet::getNormalNotes() const
        {
            return myNormalNotes;
        }


        void MetronomeTuplet::setNormalNotes( const NormalNotesPtr& value )
        {
            if( value )
            {
                myNormalNotes = value;
            }
        }


        TimeModificationNormalTypeNormalDotPtr MetronomeTuplet::getTimeModificationNormalTypeNormalDot() const
        {
            return myTimeModificationNormalTypeNormalDot;
        }


        void MetronomeTuplet::setTimeModificationNormalTypeNormalDot( const TimeModificationNormalTypeNormalDotPtr& value )
        {
            if ( value )
            {
                myTimeModificationNormalTypeNormalDot = value;
            }
        }


        bool MetronomeTuplet::getHasTimeModificationNormalTypeNormalDot() const
        {
            return myHasTimeModificationNormalTypeNormalDot;
        }


        void MetronomeTuplet::setHasTimeModificationNormalTypeNormalDot( const bool value )
        {
            myHasTimeModificationNormalTypeNormalDot = value;
        }


        bool MetronomeTuplet::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isActualNotesFound = false;
            bool isNormalNotesFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myActualNotes, isActualNotesFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myNormalNotes, isNormalNotesFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTimeModificationNormalTypeNormalDot, myHasTimeModificationNormalTypeNormalDot ) ) { continue; }
            }

            if( !isActualNotesFound )
            {
                message << "MetronomeTuplet: '" << myActualNotes->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isNormalNotesFound )
            {
                message << "MetronomeTuplet: '" << myNormalNotes->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
