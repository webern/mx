// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( LyricLanguageAttributes )

        struct LyricLanguageAttributes : public AttributesInterface
        {
        public:
            LyricLanguageAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsNMToken number;
            XsToken name;
            XmlLang lang;
            bool hasNumber;
            bool hasName;
            const bool hasLang;

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
        };
    }
}
