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

        MX_FORWARD_DECLARE_ELEMENT( FullNoteGroup )
        MX_FORWARD_DECLARE_ELEMENT( Grace )
        MX_FORWARD_DECLARE_ELEMENT( Tie )
        MX_FORWARD_DECLARE_ELEMENT( GraceNoteGroup )

        inline GraceNoteGroupPtr makeGraceNoteGroup() { return std::make_shared<GraceNoteGroup>(); }

        class GraceNoteGroup : public ElementInterface
        {
        public:
            GraceNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Grace minOccurs = 1, maxOccurs = 1 _________ */
            GracePtr getGrace() const;
            void setGrace( const GracePtr& value );

            /* _________ FullNoteGroup minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteGroupPtr getFullNoteGroup() const;
            void setFullNoteGroup( const FullNoteGroupPtr& value );

            /* _________ Tie minOccurs = 0, maxOccurs = 2 _________ */
            const TieSet& getTieSet() const;
            void addTie( const TiePtr& value );
            void removeTie( const TieSetIterConst& value );
            void clearTieSet();
            TiePtr getTie( const TieSetIterConst& setIterator ) const;
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            GracePtr myGrace;
            FullNoteGroupPtr myFullNoteGroup;
            TieSet myTieSet;
        };
    }
}
