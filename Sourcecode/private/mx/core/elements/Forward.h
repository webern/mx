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

        MX_FORWARD_DECLARE_ELEMENT( Duration )
        MX_FORWARD_DECLARE_ELEMENT( EditorialVoiceGroup )
        MX_FORWARD_DECLARE_ELEMENT( Footnote )
        MX_FORWARD_DECLARE_ELEMENT( Staff )
        MX_FORWARD_DECLARE_ELEMENT( Forward )

        inline ForwardPtr makeForward() { return std::make_shared<Forward>(); }

        class Forward : public ElementInterface
        {
        public:
            Forward();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Duration minOccurs = 1, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );

            /* _________ Footnote minOccurs = 1, maxOccurs = 1 _________ */
            EditorialVoiceGroupPtr getEditorialVoiceGroup() const;
            void setEditorialVoiceGroup( const EditorialVoiceGroupPtr& value );

            /* _________ Staff minOccurs = 0, maxOccurs = 1 _________ */
            StaffPtr getStaff() const;
            void setStaff( const StaffPtr& value );
            bool getHasStaff() const;
            void setHasStaff( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            DurationPtr myDuration;
            EditorialVoiceGroupPtr myEditorialVoiceGroup;
            StaffPtr myStaff;
            bool myHasStaff;
        };
    }
}
