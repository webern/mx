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

        MX_FORWARD_DECLARE_ELEMENT( SystemDistance )
        MX_FORWARD_DECLARE_ELEMENT( SystemDividers )
        MX_FORWARD_DECLARE_ELEMENT( SystemMargins )
        MX_FORWARD_DECLARE_ELEMENT( TopSystemDistance )
        MX_FORWARD_DECLARE_ELEMENT( SystemLayout )

        inline SystemLayoutPtr makeSystemLayout() { return std::make_shared<SystemLayout>(); }

        class SystemLayout : public ElementInterface
        {
        public:
            SystemLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ SystemMargins minOccurs = 0, maxOccurs = 1 _________ */
            SystemMarginsPtr getSystemMargins() const;
            void setSystemMargins( const SystemMarginsPtr& value );
            bool getHasSystemMargins() const;
            void setHasSystemMargins( const bool value );
            SystemDistancePtr getSystemDistance() const;
            void setSystemDistance( const SystemDistancePtr& value );
            bool getHasSystemDistance() const;
            void setHasSystemDistance( const bool value );

            /* _________ TopSystemDistance minOccurs = 0, maxOccurs = 1 _________ */
            TopSystemDistancePtr getTopSystemDistance() const;
            void setTopSystemDistance( const TopSystemDistancePtr& value );
            bool getHasTopSystemDistance() const;
            void setHasTopSystemDistance( const bool value );

            /* _________ SystemDividers minOccurs = 0, maxOccurs = 1 _________ */
            SystemDividersPtr getSystemDividers() const;
            void setSystemDividers( const SystemDividersPtr& value );
            bool getHasSystemDividers() const;
            void setHasSystemDividers( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            SystemMarginsPtr mySystemMargins;
            bool myHasSystemMargins;
            SystemDistancePtr mySystemDistance;
            bool myHasSystemDistance;
            TopSystemDistancePtr myTopSystemDistance;
            bool myHasTopSystemDistance;
            SystemDividersPtr mySystemDividers;
            bool myHasSystemDividers;
        };
    }
}
