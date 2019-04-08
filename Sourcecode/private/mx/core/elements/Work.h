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

        MX_FORWARD_DECLARE_ELEMENT( Opus )
        MX_FORWARD_DECLARE_ELEMENT( WorkNumber )
        MX_FORWARD_DECLARE_ELEMENT( WorkTitle )
        MX_FORWARD_DECLARE_ELEMENT( Work )

        inline WorkPtr makeWork() { return std::make_shared<Work>(); }

        class Work : public ElementInterface
        {
        public:
            Work();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ WorkNumber minOccurs = 0, maxOccurs = 1 _________ */
            WorkNumberPtr getWorkNumber() const;
            void setWorkNumber( const WorkNumberPtr& value );
            bool getHasWorkNumber() const;
            void setHasWorkNumber( const bool value );

            /* _________ WorkTitle minOccurs = 0, maxOccurs = 1 _________ */
            WorkTitlePtr getWorkTitle() const;
            void setWorkTitle( const WorkTitlePtr& value );
            bool getHasWorkTitle() const;
            void setHasWorkTitle( const bool value );

            /* _________ Opus minOccurs = 0, maxOccurs = 1 _________ */
            OpusPtr getOpus() const;
            void setOpus( const OpusPtr& value );
            bool getHasOpus() const;
            void setHasOpus( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            WorkNumberPtr myWorkNumber;
            bool myHasWorkNumber;
            WorkTitlePtr myWorkTitle;
            bool myHasWorkTitle;
            OpusPtr myOpus;
            bool myHasOpus;
        };
    }
}
