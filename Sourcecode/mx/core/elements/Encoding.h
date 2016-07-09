// MusicXML Class Library v0.2
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
        
        MX_FORWARD_DECLARE_ELEMENT( EncodingChoice )
        MX_FORWARD_DECLARE_ELEMENT( Encoding )
        
        inline EncodingPtr makeEncoding() { return std::make_shared<Encoding>(); }
        
        class Encoding : public ElementInterface
        {
        public:
            Encoding();
            
            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            
            /* _________ EncodingChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const EncodingChoiceSet& getEncodingChoiceSet() const;
            void addEncodingChoice( const EncodingChoicePtr& value );
            void removeEncodingChoice( const EncodingChoiceSetIterConst& value );
            void clearEncodingChoiceSet();
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            EncodingChoiceSet myEncodingChoiceSet;
        };
    }
}
