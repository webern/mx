// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ScorePart.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Group.h"
#include "mx/core/elements/Identification.h"
#include "mx/core/elements/MidiDeviceInstrumentGroup.h"
#include "mx/core/elements/PartAbbreviation.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartName.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/ScoreInstrument.h"
#include "mx/core/elements/MidiDevice.h"
#include "mx/core/elements/MidiInstrument.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ScorePart::ScorePart()
        :myAttributes( std::make_shared<ScorePartAttributes>() )
        ,myIdentification( makeIdentification() )
        ,myHasIdentification( false )
        ,myPartName( makePartName() )
        ,myPartNameDisplay( makePartNameDisplay() )
        ,myHasPartNameDisplay( false )
        ,myPartAbbreviation( makePartAbbreviation() )
        ,myHasPartAbbreviation( false )
        ,myPartAbbreviationDisplay( makePartAbbreviationDisplay() )
        ,myHasPartAbbreviationDisplay( false )
        ,myGroupSet()
        ,myScoreInstrumentSet()
        ,myMidiDeviceInstrumentGroupSet()
        {}


        bool ScorePart::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& ScorePart::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& ScorePart::streamName( std::ostream& os ) const
        {
            os << "score-part";
            return os;
        }


        bool ScorePart::hasContents() const
        {
            return true;
        }


        std::ostream& ScorePart::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasIdentification )
            {
                os << std::endl;
                myIdentification->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myPartName->toStream( os, indentLevel+1 );
            if ( myHasPartNameDisplay )
            {
                os << std::endl;
                myPartNameDisplay->toStream( os, indentLevel+1 );
            }
            if ( myHasPartAbbreviation )
            {
                os << std::endl;
                myPartAbbreviation->toStream( os, indentLevel+1 );
            }
            if ( myHasPartAbbreviationDisplay )
            {
                os << std::endl;
                myPartAbbreviationDisplay->toStream( os, indentLevel+1 );
            }
            for ( auto x : myGroupSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myScoreInstrumentSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myMidiDeviceInstrumentGroupSet )
            {
                if ( x->hasContents() )
                {
                    os << std::endl;
                    x->streamContents( os, indentLevel+1, isOneLineOnly );
                }
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        ScorePartAttributesPtr ScorePart::getAttributes() const
        {
            return myAttributes;
        }


        void ScorePart::setAttributes( const ScorePartAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        IdentificationPtr ScorePart::getIdentification() const
        {
            return myIdentification;
        }


        void ScorePart::setIdentification( const IdentificationPtr& value )
        {
            if( value )
            {
                myIdentification = value;
            }
        }


        bool ScorePart::getHasIdentification() const
        {
            return myHasIdentification;
        }


        void ScorePart::setHasIdentification( const bool value )
        {
            myHasIdentification = value;
        }


        PartNamePtr ScorePart::getPartName() const
        {
            return myPartName;
        }


        void ScorePart::setPartName( const PartNamePtr& value )
        {
            if( value )
            {
                myPartName = value;
            }
        }


        PartNameDisplayPtr ScorePart::getPartNameDisplay() const
        {
            return myPartNameDisplay;
        }


        void ScorePart::setPartNameDisplay( const PartNameDisplayPtr& value )
        {
            if( value )
            {
                myPartNameDisplay = value;
            }
        }


        bool ScorePart::getHasPartNameDisplay() const
        {
            return myHasPartNameDisplay;
        }


        void ScorePart::setHasPartNameDisplay( const bool value )
        {
            myHasPartNameDisplay = value;
        }


        PartAbbreviationPtr ScorePart::getPartAbbreviation() const
        {
            return myPartAbbreviation;
        }


        void ScorePart::setPartAbbreviation( const PartAbbreviationPtr& value )
        {
            if( value )
            {
                myPartAbbreviation = value;
            }
        }


        bool ScorePart::getHasPartAbbreviation() const
        {
            return myHasPartAbbreviation;
        }


        void ScorePart::setHasPartAbbreviation( const bool value )
        {
            myHasPartAbbreviation = value;
        }


        PartAbbreviationDisplayPtr ScorePart::getPartAbbreviationDisplay() const
        {
            return myPartAbbreviationDisplay;
        }


        void ScorePart::setPartAbbreviationDisplay( const PartAbbreviationDisplayPtr& value )
        {
            if( value )
            {
                myPartAbbreviationDisplay = value;
            }
        }


        bool ScorePart::getHasPartAbbreviationDisplay() const
        {
            return myHasPartAbbreviationDisplay;
        }


        void ScorePart::setHasPartAbbreviationDisplay( const bool value )
        {
            myHasPartAbbreviationDisplay = value;
        }


        const GroupSet& ScorePart::getGroupSet() const
        {
            return myGroupSet;
        }


        void ScorePart::removeGroup( const GroupSetIterConst& value )
        {
            if ( value != myGroupSet.cend() )
            {
                myGroupSet.erase( value );
            }
        }


        void ScorePart::addGroup( const GroupPtr& value )
        {
            if ( value )
            {
                myGroupSet.push_back( value );
            }
        }


        void ScorePart::clearGroupSet()
        {
            myGroupSet.clear();
        }


        GroupPtr ScorePart::getGroup( const GroupSetIterConst& setIterator ) const
        {
            if( setIterator != myGroupSet.cend() )
            {
                return *setIterator;
            }
            return GroupPtr();
        }


        const ScoreInstrumentSet& ScorePart::getScoreInstrumentSet() const
        {
            return myScoreInstrumentSet;
        }


        void ScorePart::removeScoreInstrument( const ScoreInstrumentSetIterConst& value )
        {
            if ( value != myScoreInstrumentSet.cend() )
            {
                myScoreInstrumentSet.erase( value );
            }
        }


        void ScorePart::addScoreInstrument( const ScoreInstrumentPtr& value )
        {
            if ( value )
            {
                myScoreInstrumentSet.push_back( value );
            }
        }


        void ScorePart::clearScoreInstrumentSet()
        {
            myScoreInstrumentSet.clear();
        }


        ScoreInstrumentPtr ScorePart::getScoreInstrument( const ScoreInstrumentSetIterConst& setIterator ) const
        {
            if( setIterator != myScoreInstrumentSet.cend() )
            {
                return *setIterator;
            }
            return ScoreInstrumentPtr();
        }


        const MidiDeviceInstrumentGroupSet& ScorePart::getMidiDeviceInstrumentGroupSet() const
        {
            return myMidiDeviceInstrumentGroupSet;
        }


        void ScorePart::addMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupPtr& value )
        {
            if ( value )
            {
                myMidiDeviceInstrumentGroupSet.push_back( value );
            }
        }


        void ScorePart::removeMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupSetIterConst& value )
        {
            if ( value != myMidiDeviceInstrumentGroupSet.cend() )
            {
                myMidiDeviceInstrumentGroupSet.erase( value );
            }
        }


        void ScorePart::clearMidiDeviceInstrumentGroupSet()
        {
            myMidiDeviceInstrumentGroupSet.clear();
        }


        MidiDeviceInstrumentGroupPtr ScorePart::getMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupSetIterConst& setIterator ) const
        {
            if( setIterator != myMidiDeviceInstrumentGroupSet.cend() )
            {
                return *setIterator;
            }
            return MidiDeviceInstrumentGroupPtr();
        }

        
        bool ScorePart::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            bool isPartNameFound = false;
            
            ::ezxml::XElementIterator end = xelement.end();
            
            for( auto it = xelement.begin(); it != end; ++it )
            {
                importElement( message, *it, isSuccess, *myIdentification, myHasIdentification );
                importElement( message, *it, isSuccess, *myPartName, isPartNameFound );
                importElement( message, *it, isSuccess, *myPartNameDisplay, myHasPartNameDisplay );
                importElement( message, *it, isSuccess, *myPartAbbreviation, myHasPartAbbreviation );
                importElement( message, *it, isSuccess, *myPartAbbreviationDisplay, myHasPartAbbreviationDisplay );
                importElementSet( message, it, end, isSuccess, "group", myGroupSet );
                importElementSet( message, it, end, isSuccess, "score-instrument", myScoreInstrumentSet );
                importMidiDeviceInstrumentGroupSet( message, it, end, isSuccess );
            }
            
            if( !isPartNameFound )
            {
                message << "ScorePart: 'part-name' element is required but was not found" << std::endl;
            }
            
            MX_RETURN_IS_SUCCESS;
        }
        
        
        void ScorePart::importMidiDeviceInstrumentGroupSet( std::ostream& message, ::ezxml::XElementIterator& iter, ::ezxml::XElementIterator& endIter, bool& isSuccess )
        {
            bool doDecrementIter = false;
            while( iter != endIter && ( iter->getName() == "midi-device" || iter->getName() == "midi-instrument" ) )
            {
                auto item = makeMidiDeviceInstrumentGroup();
                if( iter->getName() == "midi-device" )
                {
                    item->setHasMidiDevice( true );
                    auto midiDevice = item->getMidiDevice();
                    isSuccess &= midiDevice->fromXElement( message, *iter );
                    doDecrementIter = true;
                    ++iter;
                }
                
                if( iter != endIter && iter->getName() == "midi-instrument" )
                {
                    item->setHasMidiInstrument( true );
                    auto midiInstrument = item->getMidiInstrument();
                    isSuccess &= midiInstrument->fromXElement( message, *iter );
                    importGroup( message, iter, endIter, isSuccess, midiInstrument );
                    doDecrementIter = true;
                    ++iter;
                }
                
                if( item->getHasMidiDevice() || item->getHasMidiInstrument() )
                {
                    myMidiDeviceInstrumentGroupSet.push_back( item );
                }
                
                if( iter != endIter && iter->getName() != "midi-device" && iter->getName() != "midi-instrument" )
                {
                    if( doDecrementIter )
                    {
                        --iter;
                    }
                    return;
                }
            }
            if( doDecrementIter )
            {
                --iter;
            }
        }
        
    }
}
