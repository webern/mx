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

        MX_FORWARD_DECLARE_ELEMENT( Millimeters )
        MX_FORWARD_DECLARE_ELEMENT( Tenths )
        MX_FORWARD_DECLARE_ELEMENT( Scaling )

        inline ScalingPtr makeScaling() { return std::make_shared<Scaling>(); }

        class Scaling : public ElementInterface
        {
        public:
            Scaling();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Millimeters minOccurs = 1, maxOccurs = 1 _________ */
            MillimetersPtr getMillimeters() const;
            void setMillimeters( const MillimetersPtr& value );

            /* _________ Tenths minOccurs = 1, maxOccurs = 1 _________ */
            TenthsPtr getTenths() const;
            void setTenths( const TenthsPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MillimetersPtr myMillimeters;
            TenthsPtr myTenths;
        };
    }
}
