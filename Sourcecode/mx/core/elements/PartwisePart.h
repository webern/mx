// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PartAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PartAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PartwiseMeasure )
        MX_FORWARD_DECLARE_ELEMENT( PartwisePart )

        inline PartwisePartPtr makePartwisePart() { return std::make_shared<PartwisePart>(); }

        class PartwisePart : public ElementInterface
        {
        public:
            PartwisePart();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAttributesPtr getAttributes() const;
            void setAttributes( const PartAttributesPtr& value );

            /* _________ PartwiseMeasure minOccurs = 0, maxOccurs = unbounded _________ */
            const PartwiseMeasureSet& getPartwiseMeasureSet() const;
            void addPartwiseMeasure( const PartwiseMeasurePtr& value );
            void removePartwiseMeasure( const PartwiseMeasureSetIterConst& value );
            void clearPartwiseMeasureSet();
            PartwiseMeasurePtr getPartwiseMeasure( const PartwiseMeasureSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            PartAttributesPtr myAttributes;
            PartwiseMeasureSet myPartwiseMeasureSet;
        };
    }
}
