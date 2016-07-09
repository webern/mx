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

        MX_FORWARD_DECLARE_ELEMENT( TupletDot )
        MX_FORWARD_DECLARE_ELEMENT( TupletNumber )
        MX_FORWARD_DECLARE_ELEMENT( TupletType )
        MX_FORWARD_DECLARE_ELEMENT( TupletNormal )

        inline TupletNormalPtr makeTupletNormal() { return std::make_shared<TupletNormal>(); }

        class TupletNormal : public ElementInterface
        {
        public:
            TupletNormal();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ TupletNumber minOccurs = 0, maxOccurs = 1 _________ */
            TupletNumberPtr getTupletNumber() const;
            void setTupletNumber( const TupletNumberPtr& value );
            bool getHasTupletNumber() const;
            void setHasTupletNumber( const bool value );

            /* _________ TupletType minOccurs = 0, maxOccurs = 1 _________ */
            TupletTypePtr getTupletType() const;
            void setTupletType( const TupletTypePtr& value );
            bool getHasTupletType() const;
            void setHasTupletType( const bool value );

            /* _________ TupletDot minOccurs = 0, maxOccurs = unbounded _________ */
            const TupletDotSet& getTupletDotSet() const;
            void addTupletDot( const TupletDotPtr& value );
            void removeTupletDot( const TupletDotSetIterConst& value );
            void clearTupletDotSet();
            TupletDotPtr getTupletDot( const TupletDotSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            TupletNumberPtr myTupletNumber;
            bool myHasTupletNumber;
            TupletTypePtr myTupletType;
            bool myHasTupletType;
            TupletDotSet myTupletDotSet;
        };
    }
}
