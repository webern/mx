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

        MX_FORWARD_DECLARE_ELEMENT( TechnicalChoice )
        MX_FORWARD_DECLARE_ELEMENT( Technical )

        inline TechnicalPtr makeTechnical() { return std::make_shared<Technical>(); }

        class Technical : public ElementInterface
        {
        public:
            Technical();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ TechnicalChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const TechnicalChoiceSet& getTechnicalChoiceSet() const;
            void addTechnicalChoice( const TechnicalChoicePtr& value );
            void removeTechnicalChoice( const TechnicalChoiceSetIterConst& value );
            void clearTechnicalChoiceSet();
            TechnicalChoicePtr getTechnicalChoice( const TechnicalChoiceSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            TechnicalChoiceSet myTechnicalChoiceSet;
        };
    }
}
