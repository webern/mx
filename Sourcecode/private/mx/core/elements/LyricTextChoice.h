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

        MX_FORWARD_DECLARE_ELEMENT( Extend )
        MX_FORWARD_DECLARE_ELEMENT( Humming )
        MX_FORWARD_DECLARE_ELEMENT( Laughing )
        MX_FORWARD_DECLARE_ELEMENT( SyllabicTextGroup )
        MX_FORWARD_DECLARE_ELEMENT( LyricTextChoice )

        inline LyricTextChoicePtr makeLyricTextChoice() { return std::make_shared<LyricTextChoice>(); }

        class LyricTextChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                syllabicTextGroup = 1,
                extend = 2,
                laughing = 3,
                humming = 4
            };
            LyricTextChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricTextChoice::Choice getChoice() const;
            void setChoice( const LyricTextChoice::Choice value );

            /* _________ SyllabicTextGroup minOccurs = 1, maxOccurs = 1 _________ */
            SyllabicTextGroupPtr getSyllabicTextGroup() const;
            void setSyllabicTextGroup( const SyllabicTextGroupPtr& value );

            /* _________ Extend minOccurs = 1, maxOccurs = 1 _________ */
            ExtendPtr getExtend() const;
            void setExtend( const ExtendPtr& value );

            /* _________ Laughing minOccurs = 1, maxOccurs = 1 _________ */
            LaughingPtr getLaughing() const;
            void setLaughing( const LaughingPtr& value );

            /* _________ Humming minOccurs = 1, maxOccurs = 1 _________ */
            HummingPtr getHumming() const;
            void setHumming( const HummingPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            SyllabicTextGroupPtr mySyllabicTextGroup;
            ExtendPtr myExtend;
            LaughingPtr myLaughing;
            HummingPtr myHumming;
        };
    }
}
