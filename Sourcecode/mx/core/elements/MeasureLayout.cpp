// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MeasureLayout.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MeasureDistance.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MeasureLayout::MeasureLayout()
        :myMeasureDistance( makeMeasureDistance() )
        ,myHasMeasureDistance( false )
        {}


        bool MeasureLayout::hasAttributes() const
        {
            return false;
        }


        std::ostream& MeasureLayout::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MeasureLayout::streamName( std::ostream& os ) const
        {
            os << "measure-layout";
            return os;
        }


        bool MeasureLayout::hasContents() const
        {
            return true;
        }


        std::ostream& MeasureLayout::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasMeasureDistance )
            {
                os << std::endl;
                myMeasureDistance->toStream( os, indentLevel+1 );
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        MeasureDistancePtr MeasureLayout::getMeasureDistance() const
        {
            return myMeasureDistance;
        }


        void MeasureLayout::setMeasureDistance( const MeasureDistancePtr& value )
        {
            if( value )
            {
                myMeasureDistance = value;
            }
        }


        bool MeasureLayout::getHasMeasureDistance() const
        {
            return myHasMeasureDistance;
        }


        void MeasureLayout::setHasMeasureDistance( const bool value )
        {
            myHasMeasureDistance = value;
        }


        bool MeasureLayout::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myMeasureDistance, myHasMeasureDistance ) ) { continue; }
            }

            return isSuccess;
        }

    }
}
