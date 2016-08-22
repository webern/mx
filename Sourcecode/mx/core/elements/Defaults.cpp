// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Defaults.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Appearance.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/LyricFont.h"
#include "mx/core/elements/LyricLanguage.h"
#include "mx/core/elements/MusicFont.h"
#include "mx/core/elements/Scaling.h"
#include "mx/core/elements/WordFont.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Defaults::Defaults()
        :myScaling( makeScaling() )
        ,myHasScaling( false )
        ,myLayoutGroup( makeLayoutGroup() )
        ,myAppearance( makeAppearance() )
        ,myHasAppearance( false )
        ,myMusicFont( makeMusicFont() )
        ,myHasMusicFont( false )
        ,myWordFont( makeWordFont() )
        ,myHasWordFont( false )
        ,myLyricFontSet()
        ,myLyricLanguageSet()
        {}


        bool Defaults::hasAttributes() const
        {
            return false;
        }


        std::ostream& Defaults::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Defaults::streamName( std::ostream& os ) const
        {
            os << "defaults";
            return os;
        }


        bool Defaults::hasContents() const
        {
            return myHasScaling
            || myLayoutGroup->hasContents()
            || myHasAppearance
            || myHasMusicFont
            || myHasWordFont
            || myLyricFontSet.size() > 0
            || myLyricLanguageSet.size() > 0;
        }


        std::ostream& Defaults::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                if ( myHasScaling )
                {
                    os << std::endl;
                    myScaling->toStream( os, indentLevel+1 );
                }
                if ( myLayoutGroup->hasContents() )
                {
                    os << std::endl;
                    myLayoutGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                if ( myHasAppearance )
                {
                    os << std::endl;
                    myAppearance->toStream( os, indentLevel+1 );
                }
                if ( myHasMusicFont )
                {
                    os << std::endl;
                    myMusicFont->toStream( os, indentLevel+1 );
                }
                if ( myHasWordFont )
                {
                    os << std::endl;
                    myWordFont->toStream( os, indentLevel+1 );
                }
                for ( auto x : myLyricFontSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myLyricLanguageSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        ScalingPtr Defaults::getScaling() const
        {
            return myScaling;
        }


        void Defaults::setScaling( const ScalingPtr& value )
        {
            if( value )
            {
                myScaling = value;
            }
        }


        bool Defaults::getHasScaling() const
        {
            return myHasScaling;
        }


        void Defaults::setHasScaling( const bool value )
        {
            myHasScaling = value;
        }


        LayoutGroupPtr Defaults::getLayoutGroup() const
        {
            return myLayoutGroup;
        }


        void Defaults::setLayoutGroup( const LayoutGroupPtr& value )
        {
            if ( value )
            {
                myLayoutGroup = value;
            }
        }


        AppearancePtr Defaults::getAppearance() const
        {
            return myAppearance;
        }


        void Defaults::setAppearance( const AppearancePtr& value )
        {
            if( value )
            {
                myAppearance = value;
            }
        }


        bool Defaults::getHasAppearance() const
        {
            return myHasAppearance;
        }


        void Defaults::setHasAppearance( const bool value )
        {
            myHasAppearance = value;
        }


        MusicFontPtr Defaults::getMusicFont() const
        {
            return myMusicFont;
        }


        void Defaults::setMusicFont( const MusicFontPtr& value )
        {
            if( value )
            {
                myMusicFont = value;
            }
        }


        bool Defaults::getHasMusicFont() const
        {
            return myHasMusicFont;
        }


        void Defaults::setHasMusicFont( const bool value )
        {
            myHasMusicFont = value;
        }


        WordFontPtr Defaults::getWordFont() const
        {
            return myWordFont;
        }


        void Defaults::setWordFont( const WordFontPtr& value )
        {
            if( value )
            {
                myWordFont = value;
            }
        }


        bool Defaults::getHasWordFont() const
        {
            return myHasWordFont;
        }


        void Defaults::setHasWordFont( const bool value )
        {
            myHasWordFont = value;
        }


        const LyricFontSet& Defaults::getLyricFontSet() const
        {
            return myLyricFontSet;
        }


        void Defaults::removeLyricFont( const LyricFontSetIterConst& value )
        {
            if ( value != myLyricFontSet.cend() )
            {
                myLyricFontSet.erase( value );
            }
        }


        void Defaults::addLyricFont( const LyricFontPtr& value )
        {
            if ( value )
            {
                myLyricFontSet.push_back( value );
            }
        }


        void Defaults::clearLyricFontSet()
        {
            myLyricFontSet.clear();
        }


        LyricFontPtr Defaults::getLyricFont( const LyricFontSetIterConst& setIterator ) const
        {
            if( setIterator != myLyricFontSet.cend() )
            {
                return *setIterator;
            }
            return LyricFontPtr();
        }


        const LyricLanguageSet& Defaults::getLyricLanguageSet() const
        {
            return myLyricLanguageSet;
        }


        void Defaults::removeLyricLanguage( const LyricLanguageSetIterConst& value )
        {
            if ( value != myLyricLanguageSet.cend() )
            {
                myLyricLanguageSet.erase( value );
            }
        }


        void Defaults::addLyricLanguage( const LyricLanguagePtr& value )
        {
            if ( value )
            {
                myLyricLanguageSet.push_back( value );
            }
        }


        void Defaults::clearLyricLanguageSet()
        {
            myLyricLanguageSet.clear();
        }


        LyricLanguagePtr Defaults::getLyricLanguage( const LyricLanguageSetIterConst& setIterator ) const
        {
            if( setIterator != myLyricLanguageSet.cend() )
            {
                return *setIterator;
            }
            return LyricLanguagePtr();
        }


        bool Defaults::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myScaling, myHasScaling ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myLayoutGroup );
                if ( importElement( message, *it, isSuccess, *myAppearance, myHasAppearance ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMusicFont, myHasMusicFont ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myWordFont, myHasWordFont ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "lyric-font", myLyricFontSet );
                importElementSet( message, it, endIter, isSuccess, "lyric-language", myLyricLanguageSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
