// MusicXML Class Library v0.3.0
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

        MX_FORWARD_DECLARE_ELEMENT( PageLayout )
        MX_FORWARD_DECLARE_ELEMENT( StaffLayout )
        MX_FORWARD_DECLARE_ELEMENT( SystemLayout )
        MX_FORWARD_DECLARE_ELEMENT( LayoutGroup )

        inline LayoutGroupPtr makeLayoutGroup() { return std::make_shared<LayoutGroup>(); }

        class LayoutGroup : public ElementInterface
        {
        public:
            LayoutGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PageLayout minOccurs = 0, maxOccurs = 1 _________ */
            PageLayoutPtr getPageLayout() const;
            void setPageLayout( const PageLayoutPtr& value );
            bool getHasPageLayout() const;
            void setHasPageLayout( const bool value );

            /* _________ SystemLayout minOccurs = 0, maxOccurs = 1 _________ */
            SystemLayoutPtr getSystemLayout() const;
            void setSystemLayout( const SystemLayoutPtr& value );
            bool getHasSystemLayout() const;
            void setHasSystemLayout( const bool value );

            /* _________ StaffLayout minOccurs = 0, maxOccurs = unbounded _________ */
            const StaffLayoutSet& getStaffLayoutSet() const;
            void addStaffLayout( const StaffLayoutPtr& value );
            void removeStaffLayout( const StaffLayoutSetIterConst& value );
            void clearStaffLayoutSet();
            StaffLayoutPtr getStaffLayout( const StaffLayoutSetIterConst& setIterator ) const;
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PageLayoutPtr myPageLayout;
            bool myHasPageLayout;
            SystemLayoutPtr mySystemLayout;
            bool myHasSystemLayout;
            StaffLayoutSet myStaffLayoutSet;
        };
    }
}
