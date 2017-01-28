// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/MeasureAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MeasureAttributes )
        MX_FORWARD_DECLARE_ELEMENT( TimewisePart )
        MX_FORWARD_DECLARE_ELEMENT( TimewiseMeasure )

        inline TimewiseMeasurePtr makeTimewiseMeasure() { return std::make_shared<TimewiseMeasure>(); }

        class TimewiseMeasure : public ElementInterface
        {
        public:
            TimewiseMeasure();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureAttributesPtr getAttributes() const;
            void setAttributes( const MeasureAttributesPtr& value );

            /* _________ TimewisePart minOccurs = 1, maxOccurs = unbounded _________ */
            const TimewisePartSet& getTimewisePartSet() const;
            void addTimewisePart( const TimewisePartPtr& value );
            void removeTimewisePart( const TimewisePartSetIterConst& value );
            void clearTimewisePartSet();
            TimewisePartPtr getTimewisePart( const TimewisePartSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            MeasureAttributesPtr myAttributes;
            TimewisePartSet myTimewisePartSet;
        };
    }
}
