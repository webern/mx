// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Bend.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BendAlter.h"
#include "mx/core/elements/BendChoice.h"
#include "mx/core/elements/PreBend.h"
#include "mx/core/elements/Release.h"
#include "mx/core/elements/WithBar.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Bend::Bend()
        :myAttributes( std::make_shared<BendAttributes>() )
        ,myBendAlter( makeBendAlter() )
        ,myBendChoice( makeBendChoice() )
        ,myHasBendChoice( false )
        ,myWithBar( makeWithBar() )
        ,myHasWithBar( false )
        {}


        bool Bend::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Bend::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Bend::streamName( std::ostream& os ) const
        {
            os << "bend";
            return os;
        }


        bool Bend::hasContents() const
        {
            return true;
        }


        std::ostream& Bend::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myBendAlter->toStream( os, indentLevel+1 );
            
            if( myHasBendChoice )
            {
                os << std::endl;
                myBendChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            
            if ( myHasWithBar )
            {
                os << std::endl;
                myWithBar->toStream( os, indentLevel+1 );
            }
            
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        BendAttributesPtr Bend::getAttributes() const
        {
            return myAttributes;
        }


        void Bend::setAttributes( const BendAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BendAlterPtr Bend::getBendAlter() const
        {
            return myBendAlter;
        }


        void Bend::setBendAlter( const BendAlterPtr& value )
        {
            if( value )
            {
                myBendAlter = value;
            }
        }


        BendChoicePtr Bend::getBendChoice() const
        {
            return myBendChoice;
        }


        void Bend::setBendChoice( const BendChoicePtr& value )
        {
            if ( value )
            {
                myBendChoice = value;
            }
        }
        
        
        bool Bend::getHasBendChoice() const
        {
            return myHasBendChoice;
        }
        
        
        void Bend::setHasBendChoice( const bool value )
        {
            myHasBendChoice = value;
        }
        
        

        WithBarPtr Bend::getWithBar() const
        {
            return myWithBar;
        }


        void Bend::setWithBar( const WithBarPtr& value )
        {
            if( value )
            {
                myWithBar = value;
            }
        }


        bool Bend::getHasWithBar() const
        {
            return myHasWithBar;
        }


        void Bend::setHasWithBar( const bool value )
        {
            myHasWithBar = value;
        }


        bool Bend::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isBendAlterFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myBendAlter, isBendAlterFound ) ) { continue; }

                if( checkSetChoiceMember(
                    message, *it, isSuccess, myBendChoice,
                    "pre-bend", &BendChoice::getPreBend,
                    static_cast<int>( BendChoice::Choice::preBend ) ) )
                { myHasBendChoice = true; continue; }

                if( checkSetChoiceMember(
                    message, *it, isSuccess, myBendChoice,
                    "release", &BendChoice::getRelease,
                    static_cast<int>( BendChoice::Choice::release ) ) )
                { myHasBendChoice = true; continue; }
                
                if ( importElement( message, *it, isSuccess, *myWithBar, myHasWithBar ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
