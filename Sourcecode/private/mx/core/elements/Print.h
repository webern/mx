// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PrintAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PrintAttributes )
        MX_FORWARD_DECLARE_ELEMENT( LayoutGroup )
        MX_FORWARD_DECLARE_ELEMENT( MeasureLayout )
        MX_FORWARD_DECLARE_ELEMENT( MeasureNumbering )
        MX_FORWARD_DECLARE_ELEMENT( PartAbbreviationDisplay )
        MX_FORWARD_DECLARE_ELEMENT( PartNameDisplay )
        MX_FORWARD_DECLARE_ELEMENT( Print )

        inline PrintPtr makePrint() { return std::make_shared<Print>(); }

        class Print : public ElementInterface
        {
        public:
            Print();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PrintAttributesPtr getAttributes() const;
            void setAttributes( const PrintAttributesPtr& value );

            /* _________ LayoutGroup minOccurs = 1, maxOccurs = 1 _________ */
            LayoutGroupPtr getLayoutGroup() const;
            void setLayoutGroup( const LayoutGroupPtr& value );

            /* _________ MeasureLayout minOccurs = 0, maxOccurs = 1 _________ */
            MeasureLayoutPtr getMeasureLayout() const;
            void setMeasureLayout( const MeasureLayoutPtr& value );
            bool getHasMeasureLayout() const;
            void setHasMeasureLayout( const bool value );

            /* _________ MeasureNumbering minOccurs = 0, maxOccurs = 1 _________ */
            MeasureNumberingPtr getMeasureNumbering() const;
            void setMeasureNumbering( const MeasureNumberingPtr& value );
            bool getHasMeasureNumbering() const;
            void setHasMeasureNumbering( const bool value );

            /* _________ PartNameDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartNameDisplayPtr getPartNameDisplay() const;
            void setPartNameDisplay( const PartNameDisplayPtr& value );
            bool getHasPartNameDisplay() const;
            void setHasPartNameDisplay( const bool value );

            /* _________ PartAbbreviationDisplay minOccurs = 0, maxOccurs = 1 _________ */
            PartAbbreviationDisplayPtr getPartAbbreviationDisplay() const;
            void setPartAbbreviationDisplay( const PartAbbreviationDisplayPtr& value );
            bool getHasPartAbbreviationDisplay() const;
            void setHasPartAbbreviationDisplay( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );
            
        private:
            PrintAttributesPtr myAttributes;
            LayoutGroupPtr myLayoutGroup;
            MeasureLayoutPtr myMeasureLayout;
            bool myHasMeasureLayout;
            MeasureNumberingPtr myMeasureNumbering;
            bool myHasMeasureNumbering;
            PartNameDisplayPtr myPartNameDisplay;
            bool myHasPartNameDisplay;
            PartAbbreviationDisplayPtr myPartAbbreviationDisplay;
            bool myHasPartAbbreviationDisplay;
        };
    }
}
