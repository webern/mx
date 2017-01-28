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

        MX_FORWARD_DECLARE_ELEMENT( ArticulationsChoice )
        MX_FORWARD_DECLARE_ELEMENT( Articulations )

        inline ArticulationsPtr makeArticulations() { return std::make_shared<Articulations>(); }

        class Articulations : public ElementInterface
        {
        public:
            Articulations();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ArticulationsChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const ArticulationsChoiceSet& getArticulationsChoiceSet() const;
            void addArticulationsChoice( const ArticulationsChoicePtr& value );
            void removeArticulationsChoice( const ArticulationsChoiceSetIterConst& value );
            void clearArticulationsChoiceSet();
            ArticulationsChoicePtr getArticulationsChoice( const ArticulationsChoiceSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ArticulationsChoiceSet myArticulationsChoiceSet;
        };
    }
}
