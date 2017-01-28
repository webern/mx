// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Properties.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Clef.h"
#include "mx/core/elements/Directive.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Instruments.h"
#include "mx/core/elements/Key.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/MeasureStyle.h"
#include "mx/core/elements/PartSymbol.h"
#include "mx/core/elements/StaffDetails.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/Transpose.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Properties::Properties()
        :myEditorialGroup( makeEditorialGroup() )
        ,myDivisions( makeDivisions() )
        ,myHasDivisions( false )
        ,myKeySet()
        ,myTimeSet()
        ,myStaves( makeStaves() )
        ,myHasStaves( false )
        ,myPartSymbol( makePartSymbol() )
        ,myHasPartSymbol( false )
        ,myInstruments( makeInstruments() )
        ,myHasInstruments( false )
        ,myClefSet()
        ,myStaffDetailsSet()
        ,myTransposeSet()
        ,myDirectiveSet()
        ,myMeasureStyleSet()
        {}


        bool Properties::hasAttributes() const
        {
            return false;
        }


        std::ostream& Properties::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Properties::streamName( std::ostream& os ) const
        {
            os << "attributes";
            return os;
        }


        bool Properties::hasContents() const
        {
            return myEditorialGroup->hasContents()
            || myHasDivisions
            || myKeySet.size() > 0
            || myTimeSet.size() > 0
            || myHasStaves
            || myHasPartSymbol
            || myHasInstruments
            || myClefSet.size() > 0
            || myStaffDetailsSet.size() > 0
            || myTransposeSet.size() > 0
            || myDirectiveSet.size() > 0
            || myMeasureStyleSet.size() > 0;
        }


        std::ostream& Properties::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( this->hasContents() )
            {
                if ( myEditorialGroup->hasContents() )
                {
                    os << std::endl;
                    myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                if ( myHasDivisions )
                {
                    os << std::endl;
                    myDivisions->toStream( os, indentLevel+1 );
                }
                for ( auto x : myKeySet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myTimeSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                if ( myHasStaves )
                {
                    os << std::endl;
                    myStaves->toStream( os, indentLevel+1 );
                }
                if ( myHasPartSymbol )
                {
                    os << std::endl;
                    myPartSymbol->toStream( os, indentLevel+1 );
                }
                if ( myHasInstruments )
                {
                    os << std::endl;
                    myInstruments->toStream( os, indentLevel+1 );
                }
                for ( auto x : myClefSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myStaffDetailsSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myTransposeSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myDirectiveSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myMeasureStyleSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
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


        EditorialGroupPtr Properties::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void Properties::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        DivisionsPtr Properties::getDivisions() const
        {
            return myDivisions;
        }


        void Properties::setDivisions( const DivisionsPtr& value )
        {
            if( value )
            {
                myDivisions = value;
            }
        }


        bool Properties::getHasDivisions() const
        {
            return myHasDivisions;
        }


        void Properties::setHasDivisions( const bool value )
        {
            myHasDivisions = value;
        }


        const KeySet& Properties::getKeySet() const
        {
            return myKeySet;
        }


        void Properties::removeKey( const KeySetIterConst& value )
        {
            if ( value != myKeySet.cend() )
            {
                myKeySet.erase( value );
            }
        }


        void Properties::addKey( const KeyPtr& value )
        {
            if ( value )
            {
                myKeySet.push_back( value );
            }
        }


        void Properties::clearKeySet()
        {
            myKeySet.clear();
        }


        KeyPtr Properties::getKey( const KeySetIterConst& setIterator ) const
        {
            if( setIterator != myKeySet.cend() )
            {
                return *setIterator;
            }
            return KeyPtr();
        }


        const TimeSet& Properties::getTimeSet() const
        {
            return myTimeSet;
        }


        void Properties::removeTime( const TimeSetIterConst& value )
        {
            if ( value != myTimeSet.cend() )
            {
                myTimeSet.erase( value );
            }
        }


        void Properties::addTime( const TimePtr& value )
        {
            if ( value )
            {
                myTimeSet.push_back( value );
            }
        }


        void Properties::clearTimeSet()
        {
            myTimeSet.clear();
        }


        TimePtr Properties::getTime( const TimeSetIterConst& setIterator ) const
        {
            if( setIterator != myTimeSet.cend() )
            {
                return *setIterator;
            }
            return TimePtr();
        }


        StavesPtr Properties::getStaves() const
        {
            return myStaves;
        }


        void Properties::setStaves( const StavesPtr& value )
        {
            if( value )
            {
                myStaves = value;
            }
        }


        bool Properties::getHasStaves() const
        {
            return myHasStaves;
        }


        void Properties::setHasStaves( const bool value )
        {
            myHasStaves = value;
        }


        PartSymbolPtr Properties::getPartSymbol() const
        {
            return myPartSymbol;
        }


        void Properties::setPartSymbol( const PartSymbolPtr& value )
        {
            if( value )
            {
                myPartSymbol = value;
            }
        }


        bool Properties::getHasPartSymbol() const
        {
            return myHasPartSymbol;
        }


        void Properties::setHasPartSymbol( const bool value )
        {
            myHasPartSymbol = value;
        }


        InstrumentsPtr Properties::getInstruments() const
        {
            return myInstruments;
        }


        void Properties::setInstruments( const InstrumentsPtr& value )
        {
            if( value )
            {
                myInstruments = value;
            }
        }


        bool Properties::getHasInstruments() const
        {
            return myHasInstruments;
        }


        void Properties::setHasInstruments( const bool value )
        {
            myHasInstruments = value;
        }


        const ClefSet& Properties::getClefSet() const
        {
            return myClefSet;
        }


        void Properties::removeClef( const ClefSetIterConst& value )
        {
            if ( value != myClefSet.cend() )
            {
                myClefSet.erase( value );
            }
        }


        void Properties::addClef( const ClefPtr& value )
        {
            if ( value )
            {
                myClefSet.push_back( value );
            }
        }


        void Properties::clearClefSet()
        {
            myClefSet.clear();
        }


        ClefPtr Properties::getClef( const ClefSetIterConst& setIterator ) const
        {
            if( setIterator != myClefSet.cend() )
            {
                return *setIterator;
            }
            return ClefPtr();
        }


        const StaffDetailsSet& Properties::getStaffDetailsSet() const
        {
            return myStaffDetailsSet;
        }


        void Properties::removeStaffDetails( const StaffDetailsSetIterConst& value )
        {
            if ( value != myStaffDetailsSet.cend() )
            {
                myStaffDetailsSet.erase( value );
            }
        }


        void Properties::addStaffDetails( const StaffDetailsPtr& value )
        {
            if ( value )
            {
                myStaffDetailsSet.push_back( value );
            }
        }


        void Properties::clearStaffDetailsSet()
        {
            myStaffDetailsSet.clear();
        }


        StaffDetailsPtr Properties::getStaffDetails( const StaffDetailsSetIterConst& setIterator ) const
        {
            if( setIterator != myStaffDetailsSet.cend() )
            {
                return *setIterator;
            }
            return StaffDetailsPtr();
        }


        const TransposeSet& Properties::getTransposeSet() const
        {
            return myTransposeSet;
        }


        void Properties::removeTranspose( const TransposeSetIterConst& value )
        {
            if ( value != myTransposeSet.cend() )
            {
                myTransposeSet.erase( value );
            }
        }


        void Properties::addTranspose( const TransposePtr& value )
        {
            if ( value )
            {
                myTransposeSet.push_back( value );
            }
        }


        void Properties::clearTransposeSet()
        {
            myTransposeSet.clear();
        }


        TransposePtr Properties::getTranspose( const TransposeSetIterConst& setIterator ) const
        {
            if( setIterator != myTransposeSet.cend() )
            {
                return *setIterator;
            }
            return TransposePtr();
        }


        const DirectiveSet& Properties::getDirectiveSet() const
        {
            return myDirectiveSet;
        }


        void Properties::removeDirective( const DirectiveSetIterConst& value )
        {
            if ( value != myDirectiveSet.cend() )
            {
                myDirectiveSet.erase( value );
            }
        }


        void Properties::addDirective( const DirectivePtr& value )
        {
            if ( value )
            {
                myDirectiveSet.push_back( value );
            }
        }


        void Properties::clearDirectiveSet()
        {
            myDirectiveSet.clear();
        }


        DirectivePtr Properties::getDirective( const DirectiveSetIterConst& setIterator ) const
        {
            if( setIterator != myDirectiveSet.cend() )
            {
                return *setIterator;
            }
            return DirectivePtr();
        }


        const MeasureStyleSet& Properties::getMeasureStyleSet() const
        {
            return myMeasureStyleSet;
        }


        void Properties::removeMeasureStyle( const MeasureStyleSetIterConst& value )
        {
            if ( value != myMeasureStyleSet.cend() )
            {
                myMeasureStyleSet.erase( value );
            }
        }


        void Properties::addMeasureStyle( const MeasureStylePtr& value )
        {
            if ( value )
            {
                myMeasureStyleSet.push_back( value );
            }
        }


        void Properties::clearMeasureStyleSet()
        {
            myMeasureStyleSet.clear();
        }


        MeasureStylePtr Properties::getMeasureStyle( const MeasureStyleSetIterConst& setIterator ) const
        {
            if( setIterator != myMeasureStyleSet.cend() )
            {
                return *setIterator;
            }
            return MeasureStylePtr();
        }
        
        
        bool Properties::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            xml::XElementIterator end = xelement.end();

            bool hasFootnote = false;
            bool hasLevel = false;
            
            for( auto it = xelement.begin(); it != end; ++it )
            {
                importElement( message, *it, isSuccess, *myEditorialGroup->getFootnote(), hasFootnote );
                if( hasFootnote )
                {
                    myEditorialGroup->setHasFootnote( true );
                }

                importElement( message, *it, isSuccess, *myEditorialGroup->getLevel(), hasLevel );
                if( hasLevel )
                {
                    myEditorialGroup->setHasLevel( true );
                }

                if ( importElement( message, *it, isSuccess, *myDivisions, myHasDivisions ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "key", myKeySet ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "time", myTimeSet ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myStaves, myHasStaves ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPartSymbol, myHasPartSymbol ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myInstruments, myHasInstruments ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "clef", myClefSet ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "staff-details", myStaffDetailsSet ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "transpose", myTransposeSet ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "directive", myDirectiveSet ) ) { continue; }
                if ( importElementSet( message, it, end, isSuccess, "measure-style", myMeasureStyleSet ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
