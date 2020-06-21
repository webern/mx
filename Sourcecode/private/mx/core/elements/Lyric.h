// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/LyricAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( LyricAttributes )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( EndLine )
        MX_FORWARD_DECLARE_ELEMENT( EndParagraph )
        MX_FORWARD_DECLARE_ELEMENT( LyricTextChoice )
        MX_FORWARD_DECLARE_ELEMENT( Lyric )

        inline LyricPtr makeLyric() { return std::make_shared<Lyric>(); }

        class Lyric : public ElementInterface
        {
        public:
            Lyric();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricAttributesPtr getAttributes() const;
            void setAttributes( const LyricAttributesPtr& value );

            /* _________ LyricTextChoice minOccurs = 1, maxOccurs = 1 _________ */
            LyricTextChoicePtr getLyricTextChoice() const;
            void setLyricTextChoice( const LyricTextChoicePtr& value );

            /* _________ EndLine minOccurs = 0, maxOccurs = 1 _________ */
            EndLinePtr getEndLine() const;
            void setEndLine( const EndLinePtr& value );
            bool getHasEndLine() const;
            void setHasEndLine( const bool value );

            /* _________ EndParagraph minOccurs = 0, maxOccurs = 1 _________ */
            EndParagraphPtr getEndParagraph() const;
            void setEndParagraph( const EndParagraphPtr& value );
            bool getHasEndParagraph() const;
            void setHasEndParagraph( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            LyricAttributesPtr myAttributes;
            LyricTextChoicePtr myLyricTextChoice;
            EndLinePtr myEndLine;
            bool myHasEndLine;
            EndParagraphPtr myEndParagraph;
            bool myHasEndParagraph;
            EditorialGroupPtr myEditorialGroup;
        };
    }
}
