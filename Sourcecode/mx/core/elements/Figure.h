// MusicXML Class Library v0.2
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

        MX_FORWARD_DECLARE_ELEMENT( Extend )
        MX_FORWARD_DECLARE_ELEMENT( FigureNumber )
        MX_FORWARD_DECLARE_ELEMENT( Prefix )
        MX_FORWARD_DECLARE_ELEMENT( Suffix )
        MX_FORWARD_DECLARE_ELEMENT( Figure )

        inline FigurePtr makeFigure() { return std::make_shared<Figure>(); }

        class Figure : public ElementInterface
        {
        public:
            Figure();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Prefix minOccurs = 0, maxOccurs = 1 _________ */
            PrefixPtr getPrefix() const;
            void setPrefix( const PrefixPtr& value );
            bool getHasPrefix() const;
            void setHasPrefix( const bool value );

            /* _________ FigureNumber minOccurs = 0, maxOccurs = 1 _________ */
            FigureNumberPtr getFigureNumber() const;
            void setFigureNumber( const FigureNumberPtr& value );
            bool getHasFigureNumber() const;
            void setHasFigureNumber( const bool value );

            /* _________ Suffix minOccurs = 0, maxOccurs = 1 _________ */
            SuffixPtr getSuffix() const;
            void setSuffix( const SuffixPtr& value );
            bool getHasSuffix() const;
            void setHasSuffix( const bool value );

            /* _________ Extend minOccurs = 0, maxOccurs = 1 _________ */
            ExtendPtr getExtend() const;
            void setExtend( const ExtendPtr& value );
            bool getHasExtend() const;
            void setHasExtend( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PrefixPtr myPrefix;
            bool myHasPrefix;
            FigureNumberPtr myFigureNumber;
            bool myHasFigureNumber;
            SuffixPtr mySuffix;
            bool myHasSuffix;
            ExtendPtr myExtend;
            bool myHasExtend;
        };
    }
}
