// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Harmonic.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Artificial.h"
#include "mx/core/elements/BasePitch.h"
#include "mx/core/elements/HarmonicInfoChoice.h"
#include "mx/core/elements/HarmonicTypeChoice.h"
#include "mx/core/elements/Natural.h"
#include "mx/core/elements/SoundingPitch.h"
#include "mx/core/elements/TouchingPitch.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Harmonic::Harmonic()
        :myAttributes( std::make_shared<HarmonicAttributes>() )
        ,myHarmonicTypeChoice( makeHarmonicTypeChoice() )
        ,myHasHarmonicTypeChoice( false )
        ,myHarmonicInfoChoice( makeHarmonicInfoChoice() )
        ,myHasHarmonicInfoChoice( false )
        {}


        bool Harmonic::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Harmonic::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Harmonic::streamName( std::ostream& os ) const
        {
            os << "harmonic";
            return os;
        }


        bool Harmonic::hasContents() const
        {
            return myHasHarmonicTypeChoice || myHasHarmonicInfoChoice;
        }


        std::ostream& Harmonic::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                if ( myHasHarmonicTypeChoice )
                {
                    os << std::endl;
                    myHarmonicTypeChoice->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                if ( myHasHarmonicInfoChoice )
                {
                    os << std::endl;
                    myHarmonicInfoChoice->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        HarmonicTypeChoicePtr Harmonic::getHarmonicTypeChoice() const
        {
            return myHarmonicTypeChoice;
        }


        void Harmonic::setHarmonicTypeChoice( const HarmonicTypeChoicePtr& value )
        {
            if ( value )
            {
                myHarmonicTypeChoice = value;
            }
        }


        bool Harmonic::getHasHarmonicTypeChoice() const
        {
            return myHasHarmonicTypeChoice;
        }


        void Harmonic::setHasHarmonicTypeChoice( const bool value )
        {
            myHasHarmonicTypeChoice = value;
        }


        HarmonicAttributesPtr Harmonic::getAttributes() const
        {
            return myAttributes;
        }


        void Harmonic::setAttributes( const HarmonicAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        HarmonicInfoChoicePtr Harmonic::getHarmonicInfoChoice() const
        {
            return myHarmonicInfoChoice;
        }


        void Harmonic::setHarmonicInfoChoice( const HarmonicInfoChoicePtr& value )
        {
            if ( value )
            {
                myHarmonicInfoChoice = value;
            }
        }


        bool Harmonic::getHasHarmonicInfoChoice() const
        {
            return myHasHarmonicInfoChoice;
        }


        void Harmonic::setHasHarmonicInfoChoice( const bool value )
        {
            myHasHarmonicInfoChoice = value;
        }
        
        
        bool Harmonic::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if( checkSetChoiceMember(
                    message, *it, isSuccess, myHarmonicTypeChoice,
                    "natural", &HarmonicTypeChoice::getNatural,
                    static_cast<int>( HarmonicTypeChoice::Choice::natural ) ) )
                { myHasHarmonicTypeChoice = true; continue; }
                
                if( checkSetChoiceMember(
                    message, *it, isSuccess, myHarmonicTypeChoice,
                    "artificial", &HarmonicTypeChoice::getArtificial,
                    static_cast<int>( HarmonicTypeChoice::Choice::artificial ) ) )
                { myHasHarmonicTypeChoice = true; continue; }
                
                if( checkSetChoiceMember(
                    message, *it, isSuccess, myHarmonicInfoChoice,
                    "base-pitch", &HarmonicInfoChoice::getBasePitch,
                    static_cast<int>( HarmonicInfoChoice::Choice::basePitch ) ) )
                { myHasHarmonicInfoChoice = true; continue; }
                
                if( checkSetChoiceMember(
                    message, *it, isSuccess, myHarmonicInfoChoice,
                    "touching-pitch", &HarmonicInfoChoice::getTouchingPitch,
                    static_cast<int>( HarmonicInfoChoice::Choice::touchingPitch ) ) )
                { myHasHarmonicInfoChoice = true; continue; }
                
                if( checkSetChoiceMember(
                    message, *it, isSuccess, myHarmonicInfoChoice,
                    "sounding-pitch", &HarmonicInfoChoice::getSoundingPitch,
                    static_cast<int>( HarmonicInfoChoice::Choice::soundingPitch ) ) )
                { myHasHarmonicInfoChoice = true; continue; }
                
            }
            
            return isSuccess;
        }

    }
}
