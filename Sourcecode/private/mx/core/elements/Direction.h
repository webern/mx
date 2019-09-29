// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/DirectionAttributes.h"

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

        MX_FORWARD_DECLARE_ATTRIBUTES( DirectionAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DirectionType )
        MX_FORWARD_DECLARE_ELEMENT( EditorialVoiceDirectionGroup )
        MX_FORWARD_DECLARE_ELEMENT( Offset )
        MX_FORWARD_DECLARE_ELEMENT( Sound )
        MX_FORWARD_DECLARE_ELEMENT( Staff )
        MX_FORWARD_DECLARE_ELEMENT( Direction )

        inline DirectionPtr makeDirection() { return std::make_shared<Direction>(); }

        class Direction : public ElementInterface
        {
        public:
            Direction();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DirectionAttributesPtr getAttributes() const;
            void setAttributes( const DirectionAttributesPtr& value );

            /* _________ DirectionType minOccurs = 1, maxOccurs = unbounded _________ */
            const DirectionTypeSet& getDirectionTypeSet() const;
            void addDirectionType( const DirectionTypePtr& value );
            void removeDirectionType( const DirectionTypeSetIterConst& value );
            void clearDirectionTypeSet();
            DirectionTypePtr getDirectionType( const DirectionTypeSetIterConst& setIterator ) const;

            /* _________ Offset minOccurs = 0, maxOccurs = 1 _________ */
            OffsetPtr getOffset() const;
            void setOffset( const OffsetPtr& value );
            bool getHasOffset() const;
            void setHasOffset( const bool value );

            /* _________ EditorialVoiceDirectionGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialVoiceDirectionGroupPtr getEditorialVoiceDirectionGroup() const;
            void setEditorialVoiceDirectionGroup( const EditorialVoiceDirectionGroupPtr& value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

            /* _________ Sound minOccurs = 0, maxOccurs = 1 _________ */
            SoundPtr getSound() const;
            void setSound( const SoundPtr& value );
            bool getHasSound() const;
            void setHasSound( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            DirectionAttributesPtr myAttributes;
            DirectionTypeSet myDirectionTypeSet;
            OffsetPtr myOffset;
            bool myHasOffset;
            EditorialVoiceDirectionGroupPtr myEditorialVoiceDirectionGroup;
            StaffPtr myStaff;
            bool myHasStaff;
            SoundPtr mySound;
            bool myHasSound;
            
            bool importDirectionTypeSet( std::ostream& message, ::ezxml::XElementIterator& iter, ::ezxml::XElementIterator& endIter, bool& isSuccess, bool& isFound );
            DirectionTypePtr createDirectionType( std::ostream& message, ::ezxml::XElementIterator& iter, ::ezxml::XElementIterator& endIter, bool& isSuccess );
            
            bool isDirectionType( const std::string& elementName ) const;
            bool isMultiDirectionType( const std::string& elementName ) const;
            
        };
    }
}
