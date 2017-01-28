// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/HarmonyAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( HarmonyAttributes )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( Frame )
        MX_FORWARD_DECLARE_ELEMENT( HarmonyChordGroup )
        MX_FORWARD_DECLARE_ELEMENT( Offset )
        MX_FORWARD_DECLARE_ELEMENT( Staff )
        MX_FORWARD_DECLARE_ELEMENT( Harmony )

        inline HarmonyPtr makeHarmony() { return std::make_shared<Harmony>(); }

        class Harmony : public ElementInterface
        {
        public:
            Harmony();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarmonyAttributesPtr getAttributes() const;
            void setAttributes( const HarmonyAttributesPtr& value );

            /* _________ HarmonyChordGroup minOccurs = 1, maxOccurs = unbounded _________ */
            const HarmonyChordGroupSet& getHarmonyChordGroupSet() const;
            void addHarmonyChordGroup( const HarmonyChordGroupPtr& value );
            void removeHarmonyChordGroup( const HarmonyChordGroupSetIterConst& value );
            void clearHarmonyChordGroupSet();
            HarmonyChordGroupPtr getHarmonyChordGroup( const HarmonyChordGroupSetIterConst& setIterator ) const;

            /* _________ Frame minOccurs = 0, maxOccurs = 1 _________ */
            FramePtr getFrame() const;
            void setFrame( const FramePtr& value );
            bool getHasFrame() const;
            void setHasFrame( const bool value );

            /* _________ Offset minOccurs = 0, maxOccurs = 1 _________ */
            OffsetPtr getOffset() const;
            void setOffset( const OffsetPtr& value );
            bool getHasOffset() const;
            void setHasOffset( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            HarmonyAttributesPtr myAttributes;
            HarmonyChordGroupSet myHarmonyChordGroupSet;
            FramePtr myFrame;
            bool myHasFrame;
            OffsetPtr myOffset;
            bool myHasOffset;
            EditorialGroupPtr myEditorialGroup;
            StaffPtr myStaff;
            bool myHasStaff;
            
            void addGroup( HarmonyChordGroupPtr& grp, bool& isFirst );
        };
    }
}
