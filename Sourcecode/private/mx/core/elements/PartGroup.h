// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PartGroupAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PartGroupAttributes )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( GroupAbbreviation )
        MX_FORWARD_DECLARE_ELEMENT( GroupAbbreviationDisplay )
        MX_FORWARD_DECLARE_ELEMENT( GroupBarline )
        MX_FORWARD_DECLARE_ELEMENT( GroupName )
        MX_FORWARD_DECLARE_ELEMENT( GroupNameDisplay )
        MX_FORWARD_DECLARE_ELEMENT( GroupSymbol )
        MX_FORWARD_DECLARE_ELEMENT( GroupTime )
        MX_FORWARD_DECLARE_ELEMENT( PartGroup )

        inline PartGroupPtr makePartGroup() { return std::make_shared<PartGroup>(); }

        class PartGroup : public ElementInterface
        {
        public:
            PartGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartGroupAttributesPtr getAttributes() const;
            void setAttributes( const PartGroupAttributesPtr& value );

            /* _________ GroupName minOccurs = 0, maxOccurs = 1 _________ */
            GroupNamePtr getGroupName() const;
            void setGroupName( const GroupNamePtr& value );
            bool getHasGroupName() const;
            void setHasGroupName( const bool value );

            /* _________ GroupNameDisplay minOccurs = 0, maxOccurs = 1 _________ */
            GroupNameDisplayPtr getGroupNameDisplay() const;
            void setGroupNameDisplay( const GroupNameDisplayPtr& value );
            bool getHasGroupNameDisplay() const;
            void setHasGroupNameDisplay( const bool value );

            /* _________ GroupAbbreviation minOccurs = 0, maxOccurs = 1 _________ */
            GroupAbbreviationPtr getGroupAbbreviation() const;
            void setGroupAbbreviation( const GroupAbbreviationPtr& value );
            bool getHasGroupAbbreviation() const;
            void setHasGroupAbbreviation( const bool value );

            /* _________ GroupAbbreviationDisplay minOccurs = 0, maxOccurs = 1 _________ */
            GroupAbbreviationDisplayPtr getGroupAbbreviationDisplay() const;
            void setGroupAbbreviationDisplay( const GroupAbbreviationDisplayPtr& value );
            bool getHasGroupAbbreviationDisplay() const;
            void setHasGroupAbbreviationDisplay( const bool value );

            /* _________ GroupSymbol minOccurs = 0, maxOccurs = 1 _________ */
            GroupSymbolPtr getGroupSymbol() const;
            void setGroupSymbol( const GroupSymbolPtr& value );
            bool getHasGroupSymbol() const;
            void setHasGroupSymbol( const bool value );

            /* _________ GroupBarline minOccurs = 0, maxOccurs = 1 _________ */
            GroupBarlinePtr getGroupBarline() const;
            void setGroupBarline( const GroupBarlinePtr& value );
            bool getHasGroupBarline() const;
            void setHasGroupBarline( const bool value );

            /* _________ GroupTime minOccurs = 0, maxOccurs = 1 _________ */
            GroupTimePtr getGroupTime() const;
            void setGroupTime( const GroupTimePtr& value );
            bool getHasGroupTime() const;
            void setHasGroupTime( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
            
        private:
            PartGroupAttributesPtr myAttributes;
            GroupNamePtr myGroupName;
            bool myHasGroupName;
            GroupNameDisplayPtr myGroupNameDisplay;
            bool myHasGroupNameDisplay;
            GroupAbbreviationPtr myGroupAbbreviation;
            bool myHasGroupAbbreviation;
            GroupAbbreviationDisplayPtr myGroupAbbreviationDisplay;
            bool myHasGroupAbbreviationDisplay;
            GroupSymbolPtr myGroupSymbol;
            bool myHasGroupSymbol;
            GroupBarlinePtr myGroupBarline;
            bool myHasGroupBarline;
            GroupTimePtr myGroupTime;
            bool myHasGroupTime;
            EditorialGroupPtr myEditorialGroup;
        };
    }
}
