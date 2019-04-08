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
        MX_FORWARD_DECLARE_ELEMENT( MetronomeRelationGroup )
        MX_FORWARD_DECLARE_ELEMENT( NoteRelationNote )

        inline NoteRelationNotePtr makeNoteRelationNote() { return std::make_shared<NoteRelationNote>(); }

        class NoteRelationNote : public ElementInterface
        {
        public:
            NoteRelationNote();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MetronomeNote minOccurs = 1, maxOccurs = unbounded _________ */
            const MetronomeNoteSet& getMetronomeNoteSet() const;
            void addMetronomeNote( const MetronomeNotePtr& value );
            void removeMetronomeNote( const MetronomeNoteSetIterConst& setIterator );
            void clearMetronomeNoteSet();

            /* _________ MetronomeRelationGroup minOccurs = 0, maxOccurs = 1 _________ */
            MetronomeRelationGroupPtr getMetronomeRelationGroup() const;
            void setMetronomeRelationGroup( const MetronomeRelationGroupPtr& value );
            bool getHasMetronomeRelationGroup() const;
            void setHasMetronomeRelationGroup( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            MetronomeNoteSet myMetronomeNoteSet;
            MetronomeRelationGroupPtr myMetronomeRelationGroup;
            bool myHasMetronomeRelationGroup;
        };
    }
}
