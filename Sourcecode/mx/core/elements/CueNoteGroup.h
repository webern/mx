// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ELEMENT( Cue )
        MX_FORWARD_DECLARE_ELEMENT( Duration )
        MX_FORWARD_DECLARE_ELEMENT( FullNoteGroup )
        MX_FORWARD_DECLARE_ELEMENT( CueNoteGroup )

        inline CueNoteGroupPtr makeCueNoteGroup() { return std::make_shared<CueNoteGroup>(); }

        class CueNoteGroup : public ElementInterface
        {
        public:
            CueNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Cue minOccurs = 1, maxOccurs = 1 _________ */
            CuePtr getCue() const;
            void setCue( const CuePtr& value );

            /* _________ FullNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteGroupPtr getFullNoteGroup() const;
            void setFullNoteGroup( const FullNoteGroupPtr& value );

            /* _________ Duration minOccurs = 1, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            CuePtr myCue;
            FullNoteGroupPtr myFullNoteGroup;
            DurationPtr myDuration;
        };
    }
}
