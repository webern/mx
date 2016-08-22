// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/FiguredBassAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( FiguredBassAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Duration )
        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup )
        MX_FORWARD_DECLARE_ELEMENT( Figure )
        MX_FORWARD_DECLARE_ELEMENT( FiguredBass )

        inline FiguredBassPtr makeFiguredBass() { return std::make_shared<FiguredBass>(); }

        class FiguredBass : public ElementInterface
        {
        public:
            FiguredBass();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FiguredBassAttributesPtr getAttributes() const;
            void setAttributes( const FiguredBassAttributesPtr& value );

            /* _________ Figure minOccurs = 1, maxOccurs = unbounded _________ */
            const FigureSet& getFigureSet() const;
            void addFigure( const FigurePtr& value );
            void removeFigure( const FigureSetIterConst& value );
            void clearFigureSet();
            FigurePtr getFigure( const FigureSetIterConst& setIterator ) const;

            /* _________ Duration minOccurs = 0, maxOccurs = 1 _________ */
            DurationPtr getDuration() const;
            void setDuration( const DurationPtr& value );
            bool getHasDuration() const;
            void setHasDuration( const bool value );

            /* _________ EditorialGroup minOccurs = 1, maxOccurs = 1 _________ */
            EditorialGroupPtr getEditorialGroup() const;
            void setEditorialGroup( const EditorialGroupPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            FiguredBassAttributesPtr myAttributes;
            FigureSet myFigureSet;
            DurationPtr myDuration;
            bool myHasDuration;
            EditorialGroupPtr myEditorialGroup;
        };
    }
}
