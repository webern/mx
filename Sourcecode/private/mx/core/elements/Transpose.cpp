// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Transpose.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Chromatic.h"
#include "mx/core/elements/Diatonic.h"
#include "mx/core/elements/Double.h"
#include "mx/core/elements/OctaveChange.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Transpose::Transpose()
        :myAttributes( std::make_shared<TransposeAttributes>() )
        ,myDiatonic( makeDiatonic() )
        ,myHasDiatonic( false )
        ,myChromatic( makeChromatic() )
        ,myOctaveChange( makeOctaveChange() )
        ,myHasOctaveChange( false )
        ,myDouble( makeDouble() )
        ,myHasDouble( false )
        {}


        bool Transpose::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Transpose::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Transpose::streamName( std::ostream& os ) const
        {
            os << "transpose";
            return os;
        }


        bool Transpose::hasContents() const
        {
            return true;
        }


        std::ostream& Transpose::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            if( myHasDiatonic )
            {
                os << std::endl;
                myDiatonic->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myChromatic->toStream( os, indentLevel+1 );
            if( myHasOctaveChange )
            {
                os << std::endl;
                myOctaveChange->toStream( os, indentLevel+1 );
            }
            if( myHasDouble )
            {
                os << std::endl;
                myDouble->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        TransposeAttributesPtr Transpose::getAttributes() const
        {
            return myAttributes;
        }


        void Transpose::setAttributes( const TransposeAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        DiatonicPtr Transpose::getDiatonic() const
        {
            return myDiatonic;
        }


        void Transpose::setDiatonic( const DiatonicPtr& value )
        {
            if( value )
            {
                myDiatonic = value;
            }
        }


        bool Transpose::getHasDiatonic() const
        {
            return myHasDiatonic;
        }


        void Transpose::setHasDiatonic( const bool value )
        {
            myHasDiatonic = value;
        }


        ChromaticPtr Transpose::getChromatic() const
        {
            return myChromatic;
        }


        void Transpose::setChromatic( const ChromaticPtr& value )
        {
            if( value )
            {
                myChromatic = value;
            }
        }


        OctaveChangePtr Transpose::getOctaveChange() const
        {
            return myOctaveChange;
        }


        void Transpose::setOctaveChange( const OctaveChangePtr& value )
        {
            if( value )
            {
                myOctaveChange = value;
            }
        }


        bool Transpose::getHasOctaveChange() const
        {
            return myHasOctaveChange;
        }


        void Transpose::setHasOctaveChange( const bool value )
        {
            myHasOctaveChange = value;
        }


        DoublePtr Transpose::getDouble() const
        {
            return myDouble;
        }


        void Transpose::setDouble( const DoublePtr& value )
        {
            if( value )
            {
                myDouble = value;
            }
        }


        bool Transpose::getHasDouble() const
        {
            return myHasDouble;
        }


        void Transpose::setHasDouble( const bool value )
        {
            myHasDouble = value;
        }


        bool Transpose::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isChromaticFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myDiatonic, myHasDiatonic ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myChromatic, isChromaticFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myOctaveChange, myHasOctaveChange ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myDouble, myHasDouble ) ) { continue; }
            }

            if( !isChromaticFound )
            {
                message << "Transpose: '" << myChromatic->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
