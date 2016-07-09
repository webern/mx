// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( PartGroup )
        MX_FORWARD_DECLARE_ELEMENT( ScorePart )
        MX_FORWARD_DECLARE_ELEMENT( PartGroupOrScorePart )

        inline PartGroupOrScorePartPtr makePartGroupOrScorePart() { return std::make_shared<PartGroupOrScorePart>(); }

        class PartGroupOrScorePart : public ElementInterface
        {
        public:
            enum class Choice
            {
                partGroup = 1,
                scorePart = 2
            };
            PartGroupOrScorePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            PartGroupOrScorePart::Choice getChoice() const;
            void setChoice( const PartGroupOrScorePart::Choice value );

            /* _________ PartGroup minOccurs = 1, maxPartGroupOccurs = 1 _________ */
            PartGroupPtr getPartGroup() const;
            void setPartGroup( const PartGroupPtr& value );

            /* _________ ScorePart minOccurs = 1, maxOccurs = 1 _________ */
            ScorePartPtr getScorePart() const;
            void setScorePart( const ScorePartPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            Choice myChoice;
            PartGroupPtr myPartGroup;
            ScorePartPtr myScorePart;
        };
    }
}
