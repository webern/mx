// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Barline.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BarStyle.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Ending.h"
#include "mx/core/elements/Fermata.h"
#include "mx/core/elements/Repeat.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/WavyLine.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Barline::Barline()
        :myAttributes( std::make_shared<BarlineAttributes>() )
        ,myBarStyle( makeBarStyle() )
        ,myHasBarStyle( false )
        ,myEditorialGroup( makeEditorialGroup() )
        ,myWavyLine( makeWavyLine() )
        ,myHasWavyLine( false )
        ,mySegno( makeSegno() )
        ,myHasSegno( false )
        ,myCoda( makeCoda() )
        ,myHasCoda( false )
        ,myFermataSet()
        ,myEnding( makeEnding() )
        ,myHasEnding( false )
        ,myRepeat( makeRepeat() )
        ,myHasRepeat( false )
        {}


        bool Barline::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Barline::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Barline::streamName( std::ostream& os ) const
        {
            os << "barline";
            return os;
        }


        bool Barline::hasContents() const
        {
            return myHasBarStyle
            || myEditorialGroup->hasContents()
            || myHasWavyLine
            || myHasSegno
            || myHasCoda
            || myFermataSet.size() > 0
            || myHasEnding
            || myHasRepeat;
        }


        std::ostream& Barline::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasBarStyle )
            {
                os << std::endl;
                myBarStyle->toStream( os, indentLevel+1 );
            }
            if ( myEditorialGroup->hasContents() )
            {
                os << std::endl;
                myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasWavyLine )
            {
                os << std::endl;
                myWavyLine->toStream( os, indentLevel+1 );
            }
            if ( myHasSegno )
            {
                os << std::endl;
                mySegno->toStream( os, indentLevel+1 );
            }
            if ( myHasCoda )
            {
                os << std::endl;
                myCoda->toStream( os, indentLevel+1 );
            }
            for ( auto x : myFermataSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasEnding )
            {
                os << std::endl;
                myEnding->toStream( os, indentLevel+1 );
            }
            if ( myHasRepeat )
            {
                os << std::endl;
                myRepeat->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        BarlineAttributesPtr Barline::getAttributes() const
        {
            return myAttributes;
        }


        void Barline::setAttributes( const BarlineAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BarStylePtr Barline::getBarStyle() const
        {
            return myBarStyle;
        }


        void Barline::setBarStyle( const BarStylePtr& value )
        {
            if( value )
            {
                myBarStyle = value;
            }
        }


        bool Barline::getHasBarStyle() const
        {
            return myHasBarStyle;
        }


        void Barline::setHasBarStyle( const bool value )
        {
            myHasBarStyle = value;
        }


        EditorialGroupPtr Barline::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void Barline::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        WavyLinePtr Barline::getWavyLine() const
        {
            return myWavyLine;
        }


        void Barline::setWavyLine( const WavyLinePtr& value )
        {
            if( value )
            {
                myWavyLine = value;
            }
        }


        bool Barline::getHasWavyLine() const
        {
            return myHasWavyLine;
        }


        void Barline::setHasWavyLine( const bool value )
        {
            myHasWavyLine = value;
        }


        SegnoPtr Barline::getSegno() const
        {
            return mySegno;
        }


        void Barline::setSegno( const SegnoPtr& value )
        {
            if( value )
            {
                mySegno = value;
            }
        }


        bool Barline::getHasSegno() const
        {
            return myHasSegno;
        }


        void Barline::setHasSegno( const bool value )
        {
            myHasSegno = value;
        }


        CodaPtr Barline::getCoda() const
        {
            return myCoda;
        }


        void Barline::setCoda( const CodaPtr& value )
        {
            if( value )
            {
                myCoda = value;
            }
        }


        bool Barline::getHasCoda() const
        {
            return myHasCoda;
        }


        void Barline::setHasCoda( const bool value )
        {
            myHasCoda = value;
        }


        const FermataSet& Barline::getFermataSet() const
        {
            return myFermataSet;
        }


        void Barline::removeFermata( const FermataSetIterConst& value )
        {
            if ( value != myFermataSet.cend() )
            {
                if ( myFermataSet.size() > 0 )
                {
                    myFermataSet.erase( value );
                }
            }
        }


        void Barline::addFermata( const FermataPtr& value )
        {
            if ( value )
            {
                if ( myFermataSet.size() < 2 )
                {
                    myFermataSet.push_back( value );
                }
            }
        }


        void Barline::clearFermataSet()
        {
            myFermataSet.clear();
        }


        FermataPtr Barline::getFermata( const FermataSetIterConst& setIterator ) const
        {
            if( setIterator != myFermataSet.cend() )
            {
                return *setIterator;
            }
            return FermataPtr();
        }


        EndingPtr Barline::getEnding() const
        {
            return myEnding;
        }


        void Barline::setEnding( const EndingPtr& value )
        {
            if( value )
            {
                myEnding = value;
            }
        }


        bool Barline::getHasEnding() const
        {
            return myHasEnding;
        }


        void Barline::setHasEnding( const bool value )
        {
            myHasEnding = value;
        }


        RepeatPtr Barline::getRepeat() const
        {
            return myRepeat;
        }


        void Barline::setRepeat( const RepeatPtr& value )
        {
            if( value )
            {
                myRepeat = value;
            }
        }


        bool Barline::getHasRepeat() const
        {
            return myHasRepeat;
        }


        void Barline::setHasRepeat( const bool value )
        {
            myHasRepeat = value;
        }


        bool Barline::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myBarStyle, myHasBarStyle ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialGroup );
                if ( importElement( message, *it, isSuccess, *myWavyLine, myHasWavyLine ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *mySegno, myHasSegno ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myCoda, myHasCoda ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "fermata", myFermataSet );
                if ( importElement( message, *it, isSuccess, *myEnding, myHasEnding ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myRepeat, myHasRepeat ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
