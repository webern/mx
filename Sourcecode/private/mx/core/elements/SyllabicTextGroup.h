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

        MX_FORWARD_DECLARE_ELEMENT( ElisionSyllabicTextGroup )
        MX_FORWARD_DECLARE_ELEMENT( Extend )
        MX_FORWARD_DECLARE_ELEMENT( Syllabic )
        MX_FORWARD_DECLARE_ELEMENT( Text )
        MX_FORWARD_DECLARE_ELEMENT( SyllabicTextGroup )

        inline SyllabicTextGroupPtr makeSyllabicTextGroup() { return std::make_shared<SyllabicTextGroup>(); }

        class SyllabicTextGroup : public ElementInterface
        {
        public:
            SyllabicTextGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Syllabic minOccurs = 0, maxOccurs = 1 _________ */
            SyllabicPtr getSyllabic() const;
            void setSyllabic( const SyllabicPtr& value );
            bool getHasSyllabic() const;
            void setHasSyllabic( const bool value );

            /* _________ Text minOccurs = 1, maxOccurs = 1 _________ */
            TextPtr getText() const;
            void setText( const TextPtr& value );

            /* _________ ElisionSyllabicTextGroup minOccurs = 0, maxOccurs = unbounded _________ */
            const ElisionSyllabicTextGroupSet& getElisionSyllabicTextGroupSet() const;
            void addElisionSyllabicTextGroup( const ElisionSyllabicTextGroupPtr& value );
            void removeElisionSyllabicTextGroup( const ElisionSyllabicTextGroupSetIterConst& value );
            void clearElisionSyllabicTextGroupSet();
            ElisionSyllabicTextGroupPtr getElisionSyllabicTextGroup( const ElisionSyllabicTextGroupSetIterConst& setIterator ) const;

            /* _________ Extend minOccurs = 0, maxOccurs = 1 _________ */
            ExtendPtr getExtend() const;
            void setExtend( const ExtendPtr& value );
            bool getHasExtend() const;
            void setHasExtend( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            SyllabicPtr mySyllabic;
            bool myHasSyllabic;
            TextPtr myText;
            ElisionSyllabicTextGroupSet myElisionSyllabicTextGroupSet;
            ExtendPtr myExtend;
            bool myHasExtend;
        };
    }
}
