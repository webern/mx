// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/HarmonicAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( HarmonicAttributes )
        MX_FORWARD_DECLARE_ELEMENT( HarmonicInfoChoice )
        MX_FORWARD_DECLARE_ELEMENT( HarmonicTypeChoice )
        MX_FORWARD_DECLARE_ELEMENT( Harmonic )

        inline HarmonicPtr makeHarmonic() { return std::make_shared<Harmonic>(); }

        class Harmonic : public ElementInterface
        {
        public:
            Harmonic();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarmonicAttributesPtr getAttributes() const;
            void setAttributes( const HarmonicAttributesPtr& value );

            /* _________ HarmonicTypeChoice minOccurs = 0, maxOccurs = 1 _________ */
            HarmonicTypeChoicePtr getHarmonicTypeChoice() const;
            void setHarmonicTypeChoice( const HarmonicTypeChoicePtr& value );
            bool getHasHarmonicTypeChoice() const;
            void setHasHarmonicTypeChoice( const bool value );

            /* _________ HarmonicInfoChoice minOccurs = 0, maxOccurs = 1 _________ */
            HarmonicInfoChoicePtr getHarmonicInfoChoice() const;
            void setHarmonicInfoChoice( const HarmonicInfoChoicePtr& value );
            bool getHasHarmonicInfoChoice() const;
            void setHasHarmonicInfoChoice( const bool value );
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );
            
        private:
            HarmonicAttributesPtr myAttributes;
            HarmonicTypeChoicePtr myHarmonicTypeChoice;
            bool myHasHarmonicTypeChoice;
            HarmonicInfoChoicePtr myHarmonicInfoChoice;
            bool myHasHarmonicInfoChoice;
        };
    }
}
