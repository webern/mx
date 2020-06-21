// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/FiguredBass.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Figure.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FiguredBass::FiguredBass()
        :myAttributes( std::make_shared<FiguredBassAttributes>() )
        ,myFigureSet()
        ,myDuration( makeDuration() )
        ,myHasDuration( false )
        ,myEditorialGroup( makeEditorialGroup() )
        {
            myFigureSet.push_back( makeFigure() );
        }


        bool FiguredBass::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& FiguredBass::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& FiguredBass::streamName( std::ostream& os ) const
        {
            os << "figured-bass";
            return os;
        }


        bool FiguredBass::hasContents() const
        {
            return true;
        }


        std::ostream& FiguredBass::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto x : myFigureSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasDuration )
            {
                os << std::endl;
                myDuration->toStream( os, indentLevel+1 );
            }
            if ( myEditorialGroup->hasContents() )
            {
                os << std::endl;
                myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        FiguredBassAttributesPtr FiguredBass::getAttributes() const
        {
            return myAttributes;
        }


        void FiguredBass::setAttributes( const FiguredBassAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const FigureSet& FiguredBass::getFigureSet() const
        {
            return myFigureSet;
        }


        void FiguredBass::removeFigure( const FigureSetIterConst& value )
        {
            if ( value != myFigureSet.cend() )
            {
                if ( myFigureSet.size() > 1 )
                {
                    myFigureSet.erase( value );
                }
            }
        }


        void FiguredBass::addFigure( const FigurePtr& value )
        {
            if ( value )
            {
                myFigureSet.push_back( value );
            }
        }


        void FiguredBass::clearFigureSet()
        {
            myFigureSet.clear();
            while( myFigureSet.size() < 1 )
            {
                myFigureSet.push_back( makeFigure() );
            }
        }


        FigurePtr FiguredBass::getFigure( const FigureSetIterConst& setIterator ) const
        {
            if( setIterator != myFigureSet.cend() )
            {
                return *setIterator;
            }
            return FigurePtr();
        }


        DurationPtr FiguredBass::getDuration() const
        {
            return myDuration;
        }


        void FiguredBass::setDuration( const DurationPtr& value )
        {
            if ( value )
            {
                myDuration = value;
            }
        }


        bool FiguredBass::getHasDuration() const
        {
            return myHasDuration;
        }


        void FiguredBass::setHasDuration( const bool value )
        {
            myHasDuration = value;
        }


        EditorialGroupPtr FiguredBass::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void FiguredBass::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        bool FiguredBass::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "figure", myFigureSet );
                if ( importElement( message, *it, isSuccess, *myDuration, myHasDuration ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialGroup );
            }

            return isSuccess;
        }

    }
}
