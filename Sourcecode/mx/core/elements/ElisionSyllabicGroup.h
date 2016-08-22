// MusicXML Class Library v0.3.0
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

        MX_FORWARD_DECLARE_ELEMENT( Elision )
        MX_FORWARD_DECLARE_ELEMENT( Syllabic )
        MX_FORWARD_DECLARE_ELEMENT( ElisionSyllabicGroup )

        inline ElisionSyllabicGroupPtr makeElisionSyllabicGroup() { return std::make_shared<ElisionSyllabicGroup>(); }

        class ElisionSyllabicGroup : public ElementInterface
        {
        public:
            ElisionSyllabicGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Elision minOccurs = 1, maxOccurs = 1 _________ */
            ElisionPtr getElision() const;
            void setElision( const ElisionPtr& value );

            /* _________ Syllabic minOccurs = 0, maxOccurs = 1 _________ */
            SyllabicPtr getSyllabic() const;
            void setSyllabic( const SyllabicPtr& value );
            bool getHasSyllabic() const;
            void setHasSyllabic( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ElisionPtr myElision;
            SyllabicPtr mySyllabic;
            bool myHasSyllabic;
        };
    }
}
