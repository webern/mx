// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/FeatureAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( FeatureAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Feature )

        inline FeaturePtr makeFeature() { return std::make_shared<Feature>(); }
		inline FeaturePtr makeFeature( const XsString& value ) { return std::make_shared<Feature>( value ); }
		inline FeaturePtr makeFeature( XsString&& value ) { return std::make_shared<Feature>( std::move( value ) ); }

        class Feature : public ElementInterface
        {
        public:
            Feature();
            Feature( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FeatureAttributesPtr getAttributes() const;
            void setAttributes( const FeatureAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            FeatureAttributesPtr myAttributes;
        };
    }
}
