// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Figure.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Extend.h"
#include "mx/core/elements/FigureNumber.h"
#include "mx/core/elements/Prefix.h"
#include "mx/core/elements/Suffix.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Figure::Figure()
        :myPrefix( makePrefix() )
        ,myHasPrefix( false )
        ,myFigureNumber( makeFigureNumber() )
        ,myHasFigureNumber( false )
        ,mySuffix( makeSuffix() )
        ,myHasSuffix( false )
        ,myExtend( makeExtend() )
        ,myHasExtend( false )
        {}


        bool Figure::hasAttributes() const
        {
            return false;
        }


        std::ostream& Figure::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Figure::streamName( std::ostream& os ) const
        {
            os << "figure";
            return os;
        }


        bool Figure::hasContents() const
        {
            return myHasPrefix || myHasFigureNumber || myHasSuffix || myHasExtend;
        }


        std::ostream& Figure::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasPrefix )
            {
                os << std::endl;
                myPrefix->toStream( os, indentLevel+1 );
            }
            if ( myHasFigureNumber )
            {
                os << std::endl;
                myFigureNumber->toStream( os, indentLevel+1 );
            }
            if ( myHasSuffix )
            {
                os << std::endl;
                mySuffix->toStream( os, indentLevel+1 );
            }
            if ( myHasExtend )
            {
                os << std::endl;
                myExtend->toStream( os, indentLevel+1 );
            }
            if ( hasContents() )
            {
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PrefixPtr Figure::getPrefix() const
        {
            return myPrefix;
        }


        void Figure::setPrefix( const PrefixPtr& value )
        {
            if( value )
            {
                myPrefix = value;
            }
        }


        bool Figure::getHasPrefix() const
        {
            return myHasPrefix;
        }


        void Figure::setHasPrefix( const bool value )
        {
            myHasPrefix = value;
        }


        FigureNumberPtr Figure::getFigureNumber() const
        {
            return myFigureNumber;
        }


        void Figure::setFigureNumber( const FigureNumberPtr& value )
        {
            if( value )
            {
                myFigureNumber = value;
            }
        }


        bool Figure::getHasFigureNumber() const
        {
            return myHasFigureNumber;
        }


        void Figure::setHasFigureNumber( const bool value )
        {
            myHasFigureNumber = value;
        }


        SuffixPtr Figure::getSuffix() const
        {
            return mySuffix;
        }


        void Figure::setSuffix( const SuffixPtr& value )
        {
            if( value )
            {
                mySuffix = value;
            }
        }


        bool Figure::getHasSuffix() const
        {
            return myHasSuffix;
        }


        void Figure::setHasSuffix( const bool value )
        {
            myHasSuffix = value;
        }


        ExtendPtr Figure::getExtend() const
        {
            return myExtend;
        }


        void Figure::setExtend( const ExtendPtr& value )
        {
            if( value )
            {
                myExtend = value;
            }
        }


        bool Figure::getHasExtend() const
        {
            return myHasExtend;
        }


        void Figure::setHasExtend( const bool value )
        {
            myHasExtend = value;
        }


        bool Figure::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myPrefix, myHasPrefix ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myFigureNumber, myHasFigureNumber ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *mySuffix, myHasSuffix ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myExtend, myHasExtend ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
