// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Enums.h"

#include <ostream>
#include <string>

namespace mx
{
    namespace core
    {















        // DistanceTypeEnum ////////////////////////////////////////////////////////////////////////

        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value, bool& success )
        {
            success = true;
            if ( value == "beam" ) { return DistanceTypeEnum::beam; }
            else if ( value == "hyphen" ) { return DistanceTypeEnum::hyphen; }
            else if ( value == "other" ) { return DistanceTypeEnum::other; }
            success = false;
            return DistanceTypeEnum::other;
        }

        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value )
        {
            bool success = true;
            return parseDistanceTypeEnum( value, success );
        }

        std::string toString( const DistanceTypeEnum value )
        {
            switch ( value )
            {
                case DistanceTypeEnum::beam: { return "beam"; }
                case DistanceTypeEnum::hyphen: { return "hyphen"; }
                case DistanceTypeEnum::other: { return "other"; }
                default: break;
            }
            return "default";
        }

        std::ostream& toStream( std::ostream& os, const DistanceTypeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DistanceTypeEnum value )
        {
            return toStream( os, value );
        }

        DistanceType::DistanceType( const DistanceTypeEnum value )
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }

        DistanceType::DistanceType( const std::string& value )
        :myEnum( DistanceTypeEnum::other )
        ,myCustomValue( value )
        {
            setValue( value );
        }

        DistanceType::DistanceType()
        :myEnum( DistanceTypeEnum::beam )
        ,myCustomValue( "" )
        {
            setValue( DistanceTypeEnum::beam );
        }

        DistanceTypeEnum DistanceType::getValue() const
        {
            return myEnum;
        }

        std::string DistanceType::getValueString() const
        {
            if ( myEnum != DistanceTypeEnum::other )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void DistanceType::setValue( const DistanceTypeEnum value )
        {
            myEnum = value;
        }

        void DistanceType::setValue( const std::string& value )
        {
            bool found = false;
            DistanceTypeEnum temp = parseDistanceTypeEnum( value, found );
            if ( found )
            {
                myEnum = temp;
            }
            else
            {
                setValue( DistanceTypeEnum::other );
                myCustomValue = value;
            }
        }

        DistanceType parseDistanceType( const std::string& value )
        {
            return DistanceType( value );
        }
        
        std::string toString( const DistanceType& value )
        {
            return value.getValueString();
        }
        
        std::ostream& toStream( std::ostream& os, const DistanceType& value )
        {
            return os << toString( value );
        }
        
        std::ostream& operator<<( std::ostream& os, const DistanceType& value )
        {
            return toStream( os, value );
        }

        // DynamicsEnum ////////////////////////////////////////////////////////////////////////////

        DynamicsEnum parseDynamicsEnum( const std::string& value, bool& success )
        {
            success = true;
            if ( value == "p" ) { return DynamicsEnum::p; }
            else if ( value == "pp" ) { return DynamicsEnum::pp; }
            else if ( value == "ppp" ) { return DynamicsEnum::ppp; }
            else if ( value == "pppp" ) { return DynamicsEnum::pppp; }
            else if ( value == "ppppp" ) { return DynamicsEnum::ppppp; }
            else if ( value == "pppppp" ) { return DynamicsEnum::pppppp; }
            else if ( value == "f" ) { return DynamicsEnum::f; }
            else if ( value == "ff" ) { return DynamicsEnum::ff; }
            else if ( value == "fff" ) { return DynamicsEnum::fff; }
            else if ( value == "ffff" ) { return DynamicsEnum::ffff; }
            else if ( value == "fffff" ) { return DynamicsEnum::fffff; }
            else if ( value == "ffffff" ) { return DynamicsEnum::ffffff; }
            else if ( value == "mp" ) { return DynamicsEnum::mp; }
            else if ( value == "mf" ) { return DynamicsEnum::mf; }
            else if ( value == "sf" ) { return DynamicsEnum::sf; }
            else if ( value == "sfp" ) { return DynamicsEnum::sfp; }
            else if ( value == "sfpp" ) { return DynamicsEnum::sfpp; }
            else if ( value == "fp" ) { return DynamicsEnum::fp; }
            else if ( value == "rf" ) { return DynamicsEnum::rf; }
            else if ( value == "rfz" ) { return DynamicsEnum::rfz; }
            else if ( value == "sfz" ) { return DynamicsEnum::sfz; }
            else if ( value == "sffz" ) { return DynamicsEnum::sffz; }
            else if ( value == "fz" ) { return DynamicsEnum::fz; }
            else if ( value == "other-dynamics" ) { return DynamicsEnum::otherDynamics; }
            success = false;
            return DynamicsEnum::otherDynamics;
        }

        DynamicsEnum parseDynamicsEnum( const std::string& value )
        {
            bool success = true;
            return parseDynamicsEnum( value, success );
        }

        std::string toString( const DynamicsEnum value )
        {
            switch ( value )
            {
                case DynamicsEnum::p: { return "p"; }
                case DynamicsEnum::pp: { return "pp"; }
                case DynamicsEnum::ppp: { return "ppp"; }
                case DynamicsEnum::pppp: { return "pppp"; }
                case DynamicsEnum::ppppp: { return "ppppp"; }
                case DynamicsEnum::pppppp: { return "pppppp"; }
                case DynamicsEnum::f: { return "f"; }
                case DynamicsEnum::ff: { return "ff"; }
                case DynamicsEnum::fff: { return "fff"; }
                case DynamicsEnum::ffff: { return "ffff"; }
                case DynamicsEnum::fffff: { return "fffff"; }
                case DynamicsEnum::ffffff: { return "ffffff"; }
                case DynamicsEnum::mp: { return "mp"; }
                case DynamicsEnum::mf: { return "mf"; }
                case DynamicsEnum::sf: { return "sf"; }
                case DynamicsEnum::sfp: { return "sfp"; }
                case DynamicsEnum::sfpp: { return "sfpp"; }
                case DynamicsEnum::fp: { return "fp"; }
                case DynamicsEnum::rf: { return "rf"; }
                case DynamicsEnum::rfz: { return "rfz"; }
                case DynamicsEnum::sfz: { return "sfz"; }
                case DynamicsEnum::sffz: { return "sffz"; }
                case DynamicsEnum::fz: { return "fz"; }
                case DynamicsEnum::otherDynamics: { return "other-dynamics"; }
                default: break;
            }
            return "default";
        }

        std::ostream& toStream( std::ostream& os, const DynamicsEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const DynamicsEnum value )
        {
            return toStream( os, value );
        }

        DynamicsValue::DynamicsValue( const DynamicsEnum value )
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }

        DynamicsValue::DynamicsValue( const std::string& value )
        :myEnum( DynamicsEnum::otherDynamics )
        ,myCustomValue( value )
        {
            setValue( value );
        }

        DynamicsValue::DynamicsValue()
        :myEnum( DynamicsEnum::OtherDynamic )
        ,myCustomValue( "" )
        {
            setValue( DynamicsEnum::OtherDynamic );
        }

        DynamicsEnum DynamicsValue::getValue() const
        {
            return myEnum;
        }

        std::string DynamicsValue::getValueString() const
        {
            if ( myEnum != DynamicsEnum::otherDynamics )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void DynamicsValue::setValue( const DynamicsEnum value )
        {
            myEnum = value;
        }

        void DynamicsValue::setValue( const std::string& value )
        {
            bool found = false;
            DynamicsEnum temp = parseDynamicsEnum( value, found );
            if ( found )
            {
                myEnum = temp;
            }
            else
            {
                setValue( DynamicsEnum::otherDynamics );
                myCustomValue = value;
            }
        }

        DynamicsValue parseDynamicsValue( const std::string& value )
        {
            return DynamicsValue( value );
        }
        
        std::string toString( const DynamicsValue& value )
        {
            return value.getValueString();
        }
        
        std::ostream& toStream( std::ostream& os, const DynamicsValue& value )
        {
            return os << toString( value );
        }
        
        std::ostream& operator<<( std::ostream& os, const DynamicsValue& value )
        {
            return toStream( os, value );
        }




















        // LineWidthTypeEnum ///////////////////////////////////////////////////////////////////////

        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value, bool& success )
        {
            success = true;
            if ( value == "beam" ) { return LineWidthTypeEnum::beam; }
            else if ( value == "bracket" ) { return LineWidthTypeEnum::bracket; }
            else if ( value == "dashes" ) { return LineWidthTypeEnum::dashes; }
            else if ( value == "enclosure" ) { return LineWidthTypeEnum::enclosure; }
            else if ( value == "ending" ) { return LineWidthTypeEnum::ending; }
            else if ( value == "extend" ) { return LineWidthTypeEnum::extend; }
            else if ( value == "heavy barline" ) { return LineWidthTypeEnum::heavyBarline; }
            else if ( value == "leger" ) { return LineWidthTypeEnum::leger; }
            else if ( value == "light barline" ) { return LineWidthTypeEnum::lightBarline; }
            else if ( value == "octave shift" ) { return LineWidthTypeEnum::octaveShift; }
            else if ( value == "pedal" ) { return LineWidthTypeEnum::pedal; }
            else if ( value == "slur middle" ) { return LineWidthTypeEnum::slurMiddle; }
            else if ( value == "slur tip" ) { return LineWidthTypeEnum::slurTip; }
            else if ( value == "staff" ) { return LineWidthTypeEnum::staff; }
            else if ( value == "stem" ) { return LineWidthTypeEnum::stem; }
            else if ( value == "tie middle" ) { return LineWidthTypeEnum::tieMiddle; }
            else if ( value == "tie tip" ) { return LineWidthTypeEnum::tieTip; }
            else if ( value == "tuplet bracket" ) { return LineWidthTypeEnum::tupletBracket; }
            else if ( value == "wedge" ) { return LineWidthTypeEnum::wedge; }
            else if ( value == "other" ) { return LineWidthTypeEnum::other; }
            success = false;
            return LineWidthTypeEnum::other;
        }

        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value )
        {
            bool success = true;
            return parseLineWidthTypeEnum( value, success );
        }

        std::string toString( const LineWidthTypeEnum value )
        {
            switch ( value )
            {
                case LineWidthTypeEnum::beam: { return "beam"; }
                case LineWidthTypeEnum::bracket: { return "bracket"; }
                case LineWidthTypeEnum::dashes: { return "dashes"; }
                case LineWidthTypeEnum::enclosure: { return "enclosure"; }
                case LineWidthTypeEnum::ending: { return "ending"; }
                case LineWidthTypeEnum::extend: { return "extend"; }
                case LineWidthTypeEnum::heavyBarline: { return "heavy barline"; }
                case LineWidthTypeEnum::leger: { return "leger"; }
                case LineWidthTypeEnum::lightBarline: { return "light barline"; }
                case LineWidthTypeEnum::octaveShift: { return "octave shift"; }
                case LineWidthTypeEnum::pedal: { return "pedal"; }
                case LineWidthTypeEnum::slurMiddle: { return "slur middle"; }
                case LineWidthTypeEnum::slurTip: { return "slur tip"; }
                case LineWidthTypeEnum::staff: { return "staff"; }
                case LineWidthTypeEnum::stem: { return "stem"; }
                case LineWidthTypeEnum::tieMiddle: { return "tie middle"; }
                case LineWidthTypeEnum::tieTip: { return "tie tip"; }
                case LineWidthTypeEnum::tupletBracket: { return "tuplet bracket"; }
                case LineWidthTypeEnum::wedge: { return "wedge"; }
                case LineWidthTypeEnum::other: { return "other"; }
                default: break;
            }
            return "default";
        }

        std::ostream& toStream( std::ostream& os, const LineWidthTypeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const LineWidthTypeEnum value )
        {
            return toStream( os, value );
        }

        LineWidthType::LineWidthType( const LineWidthTypeEnum value )
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }

        LineWidthType::LineWidthType( const std::string& value )
        :myEnum( LineWidthTypeEnum::other )
        ,myCustomValue( value )
        {
            setValue( value );
        }

        LineWidthType::LineWidthType()
        :myEnum( LineWidthTypeEnum::beam )
        ,myCustomValue( "" )
        {
            setValue( LineWidthTypeEnum::beam );
        }

        LineWidthTypeEnum LineWidthType::getValue() const
        {
            return myEnum;
        }

        std::string LineWidthType::getValueString() const
        {
            if ( myEnum != LineWidthTypeEnum::other )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void LineWidthType::setValue( const LineWidthTypeEnum value )
        {
            myEnum = value;
        }

        void LineWidthType::setValue( const std::string& value )
        {
            bool found = false;
            LineWidthTypeEnum temp = parseLineWidthTypeEnum( value, found );
            if ( found )
            {
                myEnum = temp;
            }
            else
            {
                setValue( LineWidthTypeEnum::other );
                myCustomValue = value;
            }
        }

        LineWidthType parseLineWidthType( const std::string& value )
        {
            return LineWidthType( value );
        }
        
        std::string toString( const LineWidthType& value )
        {
            return value.getValueString();
        }
        
        std::ostream& toStream( std::ostream& os, const LineWidthType& value )
        {
            return os << toString( value );
        }
        
        std::ostream& operator<<( std::ostream& os, const LineWidthType& value )
        {
            return toStream( os, value );
        }





        // ModeEnum ////////////////////////////////////////////////////////////////////////////////

        ModeEnum parseModeEnum( const std::string& value, bool& success )
        {
            success = true;
            if ( value == "major" ) { return ModeEnum::major; }
            else if ( value == "minor" ) { return ModeEnum::minor; }
            else if ( value == "dorian" ) { return ModeEnum::dorian; }
            else if ( value == "phrygian" ) { return ModeEnum::phrygian; }
            else if ( value == "lydian" ) { return ModeEnum::lydian; }
            else if ( value == "mixolydian" ) { return ModeEnum::mixolydian; }
            else if ( value == "aeolian" ) { return ModeEnum::aeolian; }
            else if ( value == "ionian" ) { return ModeEnum::ionian; }
            else if ( value == "locrian" ) { return ModeEnum::locrian; }
            else if ( value == "none" ) { return ModeEnum::none; }
            else if ( value == "other" ) { return ModeEnum::other; }
            success = false;
            return ModeEnum::other;
        }

        ModeEnum parseModeEnum( const std::string& value )
        {
            bool success = true;
            return parseModeEnum( value, success );
        }

        std::string toString( const ModeEnum value )
        {
            switch ( value )
            {
                case ModeEnum::major: { return "major"; }
                case ModeEnum::minor: { return "minor"; }
                case ModeEnum::dorian: { return "dorian"; }
                case ModeEnum::phrygian: { return "phrygian"; }
                case ModeEnum::lydian: { return "lydian"; }
                case ModeEnum::mixolydian: { return "mixolydian"; }
                case ModeEnum::aeolian: { return "aeolian"; }
                case ModeEnum::ionian: { return "ionian"; }
                case ModeEnum::locrian: { return "locrian"; }
                case ModeEnum::none: { return "none"; }
                case ModeEnum::other: { return "other"; }
                default: break;
            }
            return "default";
        }

        std::ostream& toStream( std::ostream& os, const ModeEnum value )
        {
            return os << toString( value );
        }

        std::ostream& operator<<( std::ostream& os, const ModeEnum value )
        {
            return toStream( os, value );
        }

        ModeValue::ModeValue( const ModeEnum value )
        :myEnum( value )
        ,myCustomValue( "" )
        {
            setValue( value );
        }

        ModeValue::ModeValue( const std::string& value )
        :myEnum( ModeEnum::other )
        ,myCustomValue( value )
        {
            setValue( value );
        }

        ModeValue::ModeValue()
        :myEnum( ModeEnum::major )
        ,myCustomValue( "" )
        {
            setValue( ModeEnum::major );
        }

        ModeEnum ModeValue::getValue() const
        {
            return myEnum;
        }

        std::string ModeValue::getValueString() const
        {
            if ( myEnum != ModeEnum::other )
            {
                return toString( myEnum );
            }

            else
            {
                return myCustomValue;
            }
        }

        void ModeValue::setValue( const ModeEnum value )
        {
            myEnum = value;
        }

        void ModeValue::setValue( const std::string& value )
        {
            bool found = false;
            ModeEnum temp = parseModeEnum( value, found );
            if ( found )
            {
                myEnum = temp;
            }
            else
            {
                setValue( ModeEnum::other );
                myCustomValue = value;
            }
        }

        ModeValue parseModeValue( const std::string& value )
        {
            return ModeValue( value );
        }
        
        std::string toString( const ModeValue& value )
        {
            return value.getValueString();
        }
        
        std::ostream& toStream( std::ostream& os, const ModeValue& value )
        {
            return os << toString( value );
        }
        
        std::ostream& operator<<( std::ostream& os, const ModeValue& value )
        {
            return toStream( os, value );
        }











































    }
}
