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

        MX_FORWARD_DECLARE_ELEMENT( NonTraditionalKey )
        MX_FORWARD_DECLARE_ELEMENT( TraditionalKey )
        MX_FORWARD_DECLARE_ELEMENT( KeyChoice )

        inline KeyChoicePtr makeKeyChoice() { return std::make_shared<KeyChoice>(); }

        class KeyChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                traditionalKey = 0,
                nonTraditionalKey = 1
            };
            KeyChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Choice getChoice() const;
            void setChoice( const Choice value );
            TraditionalKeyPtr getTraditionalKey() const;
            void setTraditionalKey( const TraditionalKeyPtr& value );
            
            const NonTraditionalKeySet& getNonTraditionalKeySet() const;
            void removeNonTraditionalKey( const NonTraditionalKeySetIterConst& value );
            void addNonTraditionalKey( const NonTraditionalKeyPtr& value );
            void clearNonTraditionalKeySet();
            

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            TraditionalKeyPtr myTraditionalKey;
            NonTraditionalKeySet myNonTraditionalKeySet;
        };
    }
}
