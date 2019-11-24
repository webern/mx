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
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );
            
        private:
            EncodingChoiceSet myEncodingChoiceSet;
        };
    }
}
