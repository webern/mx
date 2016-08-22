// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Degree.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DegreeAlter.h"
#include "mx/core/elements/DegreeType.h"
#include "mx/core/elements/DegreeValue.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Degree::Degree()
        :myAttributes( std::make_shared<DegreeAttributes>() )
        ,myDegreeValue( makeDegreeValue() )
        ,myDegreeAlter( makeDegreeAlter() )
        ,myDegreeType( makeDegreeType( DegreeTypeValue::add ) )
        {}


        bool Degree::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Degree::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Degree::streamName( std::ostream& os ) const
        {
            os << "degree";
            return os;
        }


        bool Degree::hasContents() const
        {
            return true;
        }


        std::ostream& Degree::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myDegreeValue->toStream( os, indentLevel+1 );
            os << std::endl;
            myDegreeAlter->toStream( os, indentLevel+1 );
            os << std::endl;
            myDegreeType->toStream( os, indentLevel+1 );
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        DegreeAttributesPtr Degree::getAttributes() const
        {
            return myAttributes;
        }


        void Degree::setAttributes( const DegreeAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        DegreeValuePtr Degree::getDegreeValue() const
        {
            return myDegreeValue;
        }


        void Degree::setDegreeValue( const DegreeValuePtr& value )
        {
            if( value )
            {
                myDegreeValue = value;
            }
        }


        DegreeAlterPtr Degree::getDegreeAlter() const
        {
            return myDegreeAlter;
        }


        void Degree::setDegreeAlter( const DegreeAlterPtr& value )
        {
            if( value )
            {
                myDegreeAlter = value;
            }
        }


        DegreeTypePtr Degree::getDegreeType() const
        {
            return myDegreeType;
        }


        void Degree::setDegreeType( const DegreeTypePtr& value )
        {
            if( value )
            {
                myDegreeType = value;
            }
        }


        bool Degree::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isDegreeValueFound = false;
            bool isDegreeAlterFound = false;
            bool isDegreeTypeFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myDegreeValue, isDegreeValueFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myDegreeAlter, isDegreeAlterFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myDegreeType, isDegreeTypeFound ) ) { continue; }
            }

            if( !isDegreeValueFound )
            {
                message << "Degree: '" << myDegreeValue->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isDegreeAlterFound )
            {
                message << "Degree: '" << myDegreeAlter->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isDegreeTypeFound )
            {
                message << "Degree: '" << myDegreeType->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
