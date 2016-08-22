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

        MX_FORWARD_DECLARE_ELEMENT( MusicDataChoice )
        MX_FORWARD_DECLARE_ELEMENT( MusicDataGroup )

        inline MusicDataGroupPtr makeMusicDataGroup() { return std::make_shared<MusicDataGroup>(); }

        class MusicDataGroup : public ElementInterface
        {
        public:
            MusicDataGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ MusicDataChoice minOccurs = 0, maxOccurs = unbounded _________ */
            const MusicDataChoiceSet& getMusicDataChoiceSet() const;
            void addMusicDataChoice( const MusicDataChoicePtr& value );
            void removeMusicDataChoice( const MusicDataChoiceSetIterConst& value );
            void clearMusicDataChoiceSet();
            MusicDataChoicePtr getMusicDataChoice( const MusicDataChoiceSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MusicDataChoiceSet myMusicDataChoiceSet;
        };
    }
}
