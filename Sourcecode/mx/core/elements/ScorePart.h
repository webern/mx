// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ScorePartAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    
    namespace xml
    {
        class XElementIterator;
    }
    
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ScorePartAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Group )
        MX_FORWARD_DECLARE_ELEMENT( Identification )
        MX_FORWARD_DECLARE_ELEMENT( MidiDeviceInstrumentGroup )
        MX_FORWARD_DECLARE_ELEMENT( PartAbbreviation )
        MX_FORWARD_DECLARE_ELEMENT( PartAbbreviationDisplay )
        MX_FORWARD_DECLARE_ELEMENT( PartName )
        MX_FORWARD_DECLARE_ELEMENT( PartNameDisplay )
        MX_FORWARD_DECLARE_ELEMENT( ScoreInstrument )
        MX_FORWARD_DECLARE_ELEMENT( ScorePart )

        inline ScorePartPtr makeScorePart() { return std::make_shared<ScorePart>(); }

        class ScorePart : public ElementInterface
        {
        public:
            ScorePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScorePartAttributesPtr getAttributes() const;
            void setAttributes( const ScorePartAttributesPtr& value );

            /* _________ Identification minOccurs = 0, maxOccurs = 1 _________ */
            IdentificationPtr getIdentification() const;
            void setIdentification( const IdentificationPtr& value );
            bool getHasIdentification() const;
            void setHasIdentification( const bool value );

            /* _________ PartName minOccurs = 1, maxOccurs = 1 _________ */
            PartNamePtr getPartName() const;
            void setPartName( const PartNamePtr& value );

            /* _________ PartNameDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartNameDisplayPtr getPartNameDisplay() const;
            void setPartNameDisplay( const PartNameDisplayPtr& value );
            bool getHasPartNameDisplay() const;
            void setHasPartNameDisplay( const bool value );

            /* _________ PartAbbreviation minOccurs = 0, maxOccurs = 1 _________ */
            PartAbbreviationPtr getPartAbbreviation() const;
            void setPartAbbreviation( const PartAbbreviationPtr& value );
            bool getHasPartAbbreviation() const;
            void setHasPartAbbreviation( const bool value );

            /* _________ PartAbbreviationDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartAbbreviationDisplayPtr getPartAbbreviationDisplay() const;
            void setPartAbbreviationDisplay( const PartAbbreviationDisplayPtr& value );
            bool getHasPartAbbreviationDisplay() const;
            void setHasPartAbbreviationDisplay( const bool value );

            /* _________ Group minOccurs = 0, maxOccurs = unbounded _________ */
            const GroupSet& getGroupSet() const;
            void addGroup( const GroupPtr& value );
            void removeGroup( const GroupSetIterConst& value );
            void clearGroupSet();
            GroupPtr getGroup( const GroupSetIterConst& setIterator ) const;

            /* _________ ScoreInstrument minOccurs = 0, maxOccurs = unbounded _________ */
            const ScoreInstrumentSet& getScoreInstrumentSet() const;
            void addScoreInstrument( const ScoreInstrumentPtr& value );
            void removeScoreInstrument( const ScoreInstrumentSetIterConst& value );
            void clearScoreInstrumentSet();
            ScoreInstrumentPtr getScoreInstrument( const ScoreInstrumentSetIterConst& setIterator ) const;

            /* _________ MidiDeviceInstrumentGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const MidiDeviceInstrumentGroupSet& getMidiDeviceInstrumentGroupSet() const;
            void addMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupPtr& value );
            void removeMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupSetIterConst& value );
            void clearMidiDeviceInstrumentGroupSet();
            MidiDeviceInstrumentGroupPtr getMidiDeviceInstrumentGroup( const MidiDeviceInstrumentGroupSetIterConst& setIterator ) const;
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ScorePartAttributesPtr myAttributes;
            IdentificationPtr myIdentification;
            bool myHasIdentification;
            PartNamePtr myPartName;
            PartNameDisplayPtr myPartNameDisplay;
            bool myHasPartNameDisplay;
            PartAbbreviationPtr myPartAbbreviation;
            bool myHasPartAbbreviation;
            PartAbbreviationDisplayPtr myPartAbbreviationDisplay;
            bool myHasPartAbbreviationDisplay;
            GroupSet myGroupSet;
            ScoreInstrumentSet myScoreInstrumentSet;
            MidiDeviceInstrumentGroupSet myMidiDeviceInstrumentGroupSet;
            
            void importMidiDeviceInstrumentGroupSet(
                std::ostream& message,
                xml::XElementIterator& iter,
                xml::XElementIterator& endIter,
                bool& isSuccess );
        };
    }
}
