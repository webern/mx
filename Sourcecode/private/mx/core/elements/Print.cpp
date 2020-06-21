// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Print.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/MeasureLayout.h"
#include "mx/core/elements/MeasureNumbering.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartNameDisplay.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Print::Print()
        :myAttributes( std::make_shared<PrintAttributes>() )
        ,myLayoutGroup( makeLayoutGroup() )
        ,myMeasureLayout( makeMeasureLayout() )
        ,myHasMeasureLayout( false )
        ,myMeasureNumbering( makeMeasureNumbering() )
        ,myHasMeasureNumbering( false )
        ,myPartNameDisplay( makePartNameDisplay() )
        ,myHasPartNameDisplay( false )
        ,myPartAbbreviationDisplay( makePartAbbreviationDisplay() )
        ,myHasPartAbbreviationDisplay( false )
        {}


        bool Print::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Print::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Print::streamName( std::ostream& os ) const
        {
            os << "print";
            return os;
        }


        bool Print::hasContents() const
        {
            return myLayoutGroup->hasContents()
            || myHasMeasureLayout
            || myHasMeasureNumbering
            || myHasPartNameDisplay
            || myHasPartAbbreviationDisplay;
        }


        std::ostream& Print::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                if ( myLayoutGroup->hasContents() )
                {
                    os << std::endl;
                    myLayoutGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                if ( myHasMeasureLayout )
                {
                    os << std::endl;
                    myMeasureLayout->toStream( os, indentLevel+1 );
                }
                if ( myHasMeasureNumbering )
                {
                    os << std::endl;
                    myMeasureNumbering->toStream( os, indentLevel+1 );
                }
                if ( myHasPartNameDisplay )
                {
                    os << std::endl;
                    myPartNameDisplay->toStream( os, indentLevel+1 );
                }
                if ( myHasPartAbbreviationDisplay )
                {
                    os << std::endl;
                    myPartAbbreviationDisplay->toStream( os, indentLevel+1 );
                }
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PrintAttributesPtr Print::getAttributes() const
        {
            return myAttributes;
        }


        void Print::setAttributes( const PrintAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        LayoutGroupPtr Print::getLayoutGroup() const
        {
            return myLayoutGroup;
        }


        void Print::setLayoutGroup( const LayoutGroupPtr& value )
        {
            if ( value )
            {
                myLayoutGroup = value;
            }
        }


        MeasureLayoutPtr Print::getMeasureLayout() const
        {
            return myMeasureLayout;
        }


        void Print::setMeasureLayout( const MeasureLayoutPtr& value )
        {
            if ( value )
            {
                myMeasureLayout = value;
            }
        }


        bool Print::getHasMeasureLayout() const
        {
            return myHasMeasureLayout;
        }


        void Print::setHasMeasureLayout( const bool value )
        {
            myHasMeasureLayout = value;
        }


        MeasureNumberingPtr Print::getMeasureNumbering() const
        {
            return myMeasureNumbering;
        }


        void Print::setMeasureNumbering( const MeasureNumberingPtr& value )
        {
            if ( value )
            {
                myMeasureNumbering = value;
            }
        }


        bool Print::getHasMeasureNumbering() const
        {
            return myHasMeasureNumbering;
        }


        void Print::setHasMeasureNumbering( const bool value )
        {
            myHasMeasureNumbering = value;
        }


        PartNameDisplayPtr Print::getPartNameDisplay() const
        {
            return myPartNameDisplay;
        }


        void Print::setPartNameDisplay( const PartNameDisplayPtr& value )
        {
            if ( value )
            {
                myPartNameDisplay = value;
            }
        }


        bool Print::getHasPartNameDisplay() const
        {
            return myHasPartNameDisplay;
        }


        void Print::setHasPartNameDisplay( const bool value )
        {
            myHasPartNameDisplay = value;
        }


        PartAbbreviationDisplayPtr Print::getPartAbbreviationDisplay() const
        {
            return myPartAbbreviationDisplay;
        }


        void Print::setPartAbbreviationDisplay( const PartAbbreviationDisplayPtr& value )
        {
            if ( value )
            {
                myPartAbbreviationDisplay = value;
            }
        }


        bool Print::getHasPartAbbreviationDisplay() const
        {
            return myHasPartAbbreviationDisplay;
        }


        void Print::setHasPartAbbreviationDisplay( const bool value )
        {
            myHasPartAbbreviationDisplay = value;
        }
        
        
        bool Print::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importGroup( message, it, endIter, isSuccess, myLayoutGroup );
                if ( importElement( message, *it, isSuccess, *myMeasureLayout, myHasMeasureLayout ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMeasureNumbering, myHasMeasureNumbering ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPartNameDisplay, myHasPartNameDisplay ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPartAbbreviationDisplay, myHasPartAbbreviationDisplay ) ) { continue; }
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
