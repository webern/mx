// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Harmony.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Bass.h"
#include "mx/core/elements/Degree.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Frame.h"
#include "mx/core/elements/Function.h"
#include "mx/core/elements/Inversion.h"
#include "mx/core/elements/HarmonyChordGroup.h"
#include "mx/core/elements/Kind.h"
#include "mx/core/elements/Root.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Staff.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Harmony::Harmony()
        :myAttributes( std::make_shared<HarmonyAttributes>() )
        ,myHarmonyChordGroupSet()
        ,myFrame( makeFrame() )
        ,myHasFrame( false )
        ,myOffset( makeOffset() )
        ,myHasOffset( false )
        ,myEditorialGroup( makeEditorialGroup() )
        ,myStaff( makeStaff() )
        ,myHasStaff( false )
        {
            myHarmonyChordGroupSet.push_back( makeHarmonyChordGroup() );
        }


        bool Harmony::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Harmony::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Harmony::streamName( std::ostream& os ) const
        {
            os << "harmony";
            return os;
        }


        bool Harmony::hasContents() const
        {
            return true;
        }


        std::ostream& Harmony::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto x : myHarmonyChordGroupSet )
            {
                os << std::endl;
                x->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasFrame )
            {
                os << std::endl;
                myFrame->toStream( os, indentLevel+1 );
            }
            if ( myHasOffset )
            {
                os << std::endl;
                myOffset->toStream( os, indentLevel+1 );
            }
            if ( myEditorialGroup->hasContents() )
            {
                os << std::endl;
                myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasStaff )
            {
                os << std::endl;
                myStaff->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        HarmonyAttributesPtr Harmony::getAttributes() const
        {
            return myAttributes;
        }


        void Harmony::setAttributes( const HarmonyAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const HarmonyChordGroupSet& Harmony::getHarmonyChordGroupSet() const
        {
            return myHarmonyChordGroupSet;
        }


        void Harmony::addHarmonyChordGroup( const HarmonyChordGroupPtr& value )
        {
            if ( value )
            {
                myHarmonyChordGroupSet.push_back( value );
            }
        }


        void Harmony::removeHarmonyChordGroup( const HarmonyChordGroupSetIterConst& value )
        {
            if ( value != myHarmonyChordGroupSet.cend() )
            {
                if( myHarmonyChordGroupSet.size() > 1 )
                {
                    myHarmonyChordGroupSet.erase( value );
                }
            }
        }


        void Harmony::clearHarmonyChordGroupSet()
        {
            myHarmonyChordGroupSet.clear();
            myHarmonyChordGroupSet.push_back( makeHarmonyChordGroup() );
        }


        HarmonyChordGroupPtr Harmony::getHarmonyChordGroup( const HarmonyChordGroupSetIterConst& setIterator ) const
        {
            if( setIterator != myHarmonyChordGroupSet.cend() )
            {
                return *setIterator;
            }
            return HarmonyChordGroupPtr();
        }


        FramePtr Harmony::getFrame() const
        {
            return myFrame;
        }


        void Harmony::setFrame( const FramePtr& value )
        {
            if ( value )
            {
                myFrame = value;
            }
        }


        bool Harmony::getHasFrame() const
        {
            return myHasFrame;
        }


        void Harmony::setHasFrame( const bool value )
        {
            myHasFrame = value;
        }


        OffsetPtr Harmony::getOffset() const
        {
            return myOffset;
        }


        void Harmony::setOffset( const OffsetPtr& value )
        {
            if ( value )
            {
                myOffset = value;
            }
        }


        bool Harmony::getHasOffset() const
        {
            return myHasOffset;
        }


        void Harmony::setHasOffset( const bool value )
        {
            myHasOffset = value;
        }


        EditorialGroupPtr Harmony::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void Harmony::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        StaffPtr Harmony::getStaff() const
        {
            return myStaff;
        }


        void Harmony::setStaff( const StaffPtr& value )
        {
            if ( value )
            {
                myStaff = value;
            }
        }


        bool Harmony::getHasStaff() const
        {
            return myHasStaff;
        }


        void Harmony::setHasStaff( const bool value )
        {
            myHasStaff = value;
        }


        bool Harmony::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isChoiceFound = false;
            bool isFirstHarmonyChordGroupAdded = false;
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                const std::string elementName = it->getName();
                if( elementName == "root" || elementName == "function" )
                {
                    bool decrementIter = false;
                    isChoiceFound = true;
                    auto item = makeHarmonyChordGroup();
                    if( elementName == "root" )
                    {
                        item->setChoice( HarmonyChordGroup::Choice::root );
                        isSuccess &= item->getRoot()->fromXElement( message, *it );
                    }
                    else
                    {
                        item->setChoice( HarmonyChordGroup::Choice::function );
                        isSuccess &= item->getFunction()->fromXElement( message, *it );
                    }
                    decrementIter = true;
                    ++it;
                    if( it == endIter || it->getName() != "kind" ) { message << "Harmony: 'kind' is a required element and is missing aborting" << std::endl; return false; }
                    isSuccess &= item->getKind()->fromXElement( message, *it );
                    decrementIter = true;
                    ++it;
                    if( it == endIter ) { addGroup( item, isFirstHarmonyChordGroupAdded ); MX_RETURN_IS_SUCCESS; }
                    if( it->getName() == "inversion" )
                    {
                        isSuccess &= item->getInversion()->fromXElement( message, *it );
                        item->setHasInversion( true );
                        decrementIter = true;
                        ++it;
                    }
                    if( it == endIter ) { addGroup( item, isFirstHarmonyChordGroupAdded ); MX_RETURN_IS_SUCCESS; }
                    if( it->getName() == "bass" )
                    {
                        isSuccess &= item->getBass()->fromXElement( message, *it );
                        item->setHasBass( true );
                        decrementIter = true;
                        ++it;
                    }
                    if( it == endIter ) { addGroup( item, isFirstHarmonyChordGroupAdded ); MX_RETURN_IS_SUCCESS; }
                    
                    while( it != endIter && it->getName() == "degree" )
                    {
                        auto degree = makeDegree();
                        isSuccess &= degree->fromXElement( message, *it );
                        item->addDegree( degree );
                        decrementIter = true;
                        ++it;
                    }
                    
                    addGroup( item, isFirstHarmonyChordGroupAdded );
                    
                    if( decrementIter )
                    {
                        --it;
                    }
                    continue;
                }

                if ( importElement( message, *it, isSuccess, *myFrame, myHasFrame ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myOffset, myHasOffset ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialGroup );
                if ( importElement( message, *it, isSuccess, *myStaff, myHasStaff ) ) { continue; }
            }

            if( !isChoiceFound )
            {
                message << "Harmony: either 'root' or 'function' is required but neither was present" << std::endl;
                isSuccess = false;
            }
            
            return isSuccess;
        }

        void Harmony::addGroup( HarmonyChordGroupPtr& grp, bool& isFirst )
        {
            if( !isFirst && myHarmonyChordGroupSet.size() == 1 )
            {
                *myHarmonyChordGroupSet.begin() = grp;
                isFirst = true;
            }
            else
            {
                myHarmonyChordGroupSet.push_back( grp );
                isFirst = true;
            }
        }
    }
}
