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

        MX_FORWARD_DECLARE_ELEMENT( MetronomeNote )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeRelation )
        MX_FORWARD_DECLARE_ELEMENT( MetronomeRelationGroup )

        inline MetronomeRelationGroupPtr makeMetronomeRelationGroup() { return std::make_shared<MetronomeRelationGroup>(); }

        class MetronomeRelationGroup : public ElementInterface
        {
        public:
            MetronomeRelationGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MetronomeRelation minOccurs = 1, maxOccurs = 1 _________ */
            MetronomeRelationPtr getMetronomeRelation() const;
            void setMetronomeRelation( const MetronomeRelationPtr& value );

            /* _________ MetronomeNote minOccurs = 1, maxOccurs = 1 _________ */
            MetronomeNotePtr getMetronomeNote() const;
            void setMetronomeNote( const MetronomeNotePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MetronomeRelationPtr myMetronomeRelation;
            MetronomeNotePtr myMetronomeNote;
        };
    }
}
