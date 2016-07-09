// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MeasureStyleChoice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MeasureStyle::MeasureStyle()
        :myAttributes( std::make_shared<MeasureStyleAttributes>() )
        ,myChoice( makeMeasureStyleChoice() )
        {}


        bool MeasureStyle::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& MeasureStyle::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& MeasureStyle::streamName( std::ostream& os ) const
        {
            os << "measure-style";
            return os;
        }


        bool MeasureStyle::hasContents() const
        {
            return true;
        }


        std::ostream& MeasureStyle::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myChoice->streamContents( os, indentLevel, isOneLineOnly );
            os << std::endl;
            return os;
        }


        MeasureStyleAttributesPtr MeasureStyle::getAttributes() const
        {
            return myAttributes;
        }


        void MeasureStyle::setAttributes( const MeasureStyleAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        MeasureStyleChoicePtr MeasureStyle::getMeasureStyleChoice() const
        {
            return myChoice;
        }


        void MeasureStyle::setMeasureStyleChoice( const MeasureStyleChoicePtr& value )
        {
            if ( value )
            {
                myChoice = value;
            }
        }


        bool MeasureStyle::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                isSuccess &= myChoice->fromXElement( message, *it );
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
