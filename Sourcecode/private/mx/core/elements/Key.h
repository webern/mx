// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/KeyAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace ezxml
{
    class XElementIterator;
}

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( KeyAttributes )
        MX_FORWARD_DECLARE_ELEMENT( KeyChoice )
        MX_FORWARD_DECLARE_ELEMENT( KeyOctave )
        MX_FORWARD_DECLARE_ELEMENT( Key )

        inline KeyPtr makeKey() { return std::make_shared<Key>(); }

        class Key : public ElementInterface
        {
        public:
            Key();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            KeyAttributesPtr getAttributes() const;
            void setAttributes( const KeyAttributesPtr& value );
            KeyChoicePtr getKeyChoice() const;
            void setKeyChoice( const KeyChoicePtr& value );
            const KeyOctaveSet& getKeyOctaveSet() const;
            void removeKeyOctave( const KeyOctaveSetIterConst& value );
            void addKeyOctave( const KeyOctavePtr& value );
            void clearKeyOctaveSet();

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            KeyAttributesPtr myAttributes;
            KeyChoicePtr myKeyChoice;
            KeyOctaveSet myKeyOctaveSet;
            
            bool importTraditionalKey( std::ostream& message, ::ezxml::XElementIterator& iter, ::ezxml::XElementIterator& endIter, bool& isSuccess );
            bool importNonTraditionalKey( std::ostream& message, ::ezxml::XElementIterator& iter, ::ezxml::XElementIterator& endIter, bool& isSuccess );
        };
    }
}
