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

        MX_FORWARD_DECLARE_ELEMENT( Appearance )
        MX_FORWARD_DECLARE_ELEMENT( LayoutGroup )
        MX_FORWARD_DECLARE_ELEMENT( LyricFont )
        MX_FORWARD_DECLARE_ELEMENT( LyricLanguage )
        MX_FORWARD_DECLARE_ELEMENT( MusicFont )
        MX_FORWARD_DECLARE_ELEMENT( Scaling )
        MX_FORWARD_DECLARE_ELEMENT( WordFont )
        MX_FORWARD_DECLARE_ELEMENT( Defaults )

        inline DefaultsPtr makeDefaults() { return std::make_shared<Defaults>(); }

        class Defaults : public ElementInterface
        {
        public:
            Defaults();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Scaling minOccurs = 0, maxOccurs = 1 _________ */
            ScalingPtr getScaling() const;
            void setScaling( const ScalingPtr& value );
            bool getHasScaling() const;
            void setHasScaling( const bool value );

            /* _________ LayoutGroup minOccurs = 1, maxOccurs = 1 _________ */
            LayoutGroupPtr getLayoutGroup() const;
            void setLayoutGroup( const LayoutGroupPtr& value );

            /* _________ Appearance minOccurs = 0, maxOccurs = 1 _________ */
            AppearancePtr getAppearance() const;
            void setAppearance( const AppearancePtr& value );
            bool getHasAppearance() const;
            void setHasAppearance( const bool value );

            /* _________ MusicFont minOccurs = 0, maxOccurs = 1 _________ */
            MusicFontPtr getMusicFont() const;
            void setMusicFont( const MusicFontPtr& value );
            bool getHasMusicFont() const;
            void setHasMusicFont( const bool value );

            /* _________ WordFont minOccurs = 0, maxOccurs = 1 _________ */
            WordFontPtr getWordFont() const;
            void setWordFont( const WordFontPtr& value );
            bool getHasWordFont() const;
            void setHasWordFont( const bool value );

            /* _________ LyricFont minOccurs = 0, maxOccurs = unbounded _________ */
            const LyricFontSet& getLyricFontSet() const;
            void addLyricFont( const LyricFontPtr& value );
            void removeLyricFont( const LyricFontSetIterConst& value );
            void clearLyricFontSet();
            LyricFontPtr getLyricFont( const LyricFontSetIterConst& setIterator ) const;

            /* _________ LyricLanguage minOccurs = 0, maxOccurs = unbounded _________ */
            const LyricLanguageSet& getLyricLanguageSet() const;
            void addLyricLanguage( const LyricLanguagePtr& value );
            void removeLyricLanguage( const LyricLanguageSetIterConst& value );
            void clearLyricLanguageSet();
            LyricLanguagePtr getLyricLanguage( const LyricLanguageSetIterConst& setIterator ) const;

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            ScalingPtr myScaling;
            bool myHasScaling;
            LayoutGroupPtr myLayoutGroup;
            AppearancePtr myAppearance;
            bool myHasAppearance;
            MusicFontPtr myMusicFont;
            bool myHasMusicFont;
            WordFontPtr myWordFont;
            bool myHasWordFont;
            LyricFontSet myLyricFontSet;
            LyricLanguageSet myLyricLanguageSet;
        };
    }
}
