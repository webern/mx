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

        MX_FORWARD_DECLARE_ELEMENT( Footnote )
        MX_FORWARD_DECLARE_ELEMENT( Level )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )

        inline EditorialGroupPtr makeEditorialGroup() { return std::make_shared<EditorialGroup>(); }

        class EditorialGroup : public ElementInterface
        {
        public:
            EditorialGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Footnote minOccurs = 0, maxOccurs = 1 _________ */
            FootnotePtr getFootnote() const;
            void setFootnote( const FootnotePtr& value );
            bool getHasFootnote() const;
            void setHasFootnote( const bool value );

            /* _________ Level minOccurs = 0, maxOccurs = 1 _________ */
            LevelPtr getLevel() const;
            void setLevel( const LevelPtr& value );
            bool getHasLevel() const;
            void setHasLevel( const bool value );
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            FootnotePtr myFootnote;
            bool myHasFootnote;
            LevelPtr myLevel;
            bool myHasLevel;
        };
    }
}
