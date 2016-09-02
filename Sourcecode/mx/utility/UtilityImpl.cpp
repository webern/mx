// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/utility/UtilityImpl.h"

using namespace mx::core;

namespace mx
{
	namespace utility
	{
        PartGroupPtr makePartGroupStartTag( const PartGroupParams& params )
        {
            auto pg = makePartGroup();
            pg->getAttributes()->type = StartStop::start;
            if( params.number >= 0 )
            {
                pg->getAttributes()->hasNumber = true;
                pg->getAttributes()->number = XsToken( std::to_string( params.number ) );
            }
            
            pg->setHasGroupName( params.name.size() > 0 );
            pg->getGroupName()->setValue( XsString( params.name ) );
            pg->setHasGroupAbbreviation( params.abbreviation.size() > 0 );
            pg->getGroupAbbreviation()->setValue( XsString( params.abbreviation ) );
            pg->setHasGroupBarline( true );
            pg->getGroupBarline()->setValue( GroupBarlineValue::no );
            if( params.isGroupBarline )
            {
                pg->getGroupBarline()->setValue( GroupBarlineValue::yes );
            }
            pg->setHasGroupSymbol( true );
            pg->getGroupSymbol()->setValue( params.groupSymbol );
            
            pg->setHasGroupNameDisplay( true );
            auto dt = makeDisplayTextOrAccidentalText();
            dt->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
            dt->getDisplayText()->setValue( XsString( params.displayName ) );
            pg->getGroupNameDisplay()->addDisplayTextOrAccidentalText( dt );
            
            pg->setHasGroupAbbreviationDisplay( true );
            auto gad = makeDisplayTextOrAccidentalText();
            gad->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
            gad->getDisplayText()->setValue( XsString( params.displayAbbreviation ) );
            pg->getGroupAbbreviationDisplay()->addDisplayTextOrAccidentalText( gad );
            return pg ;
        }
        
        bool isPartListInitialized( const MxDoc& doc )
        {
            auto partHeaderList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
            auto partwisePartSet = doc->getScorePartwise()->getPartwisePartSet();
            
            // hack - see if the default constructed ScorePart is the only existing ScorePart
            // and, if it is, see if it is in its default constructed state.  If so, then no
            // parts have been added to the document and the part list in "uninitialized"
            
            bool isInDefaultState = partHeaderList->getPartGroupOrScorePartSet().size() == 0;
            isInDefaultState &= partHeaderList->getScorePart()->getAttributes()->id.getValue() == "ID";
            isInDefaultState &= partHeaderList->getScorePart()->getHasIdentification() ==  false;
            isInDefaultState &= partHeaderList->getScorePart()->getHasPartAbbreviation() ==  false;
            isInDefaultState &= partHeaderList->getScorePart()->getHasPartAbbreviationDisplay() ==  false;
            isInDefaultState &= partHeaderList->getScorePart()->getHasPartNameDisplay() ==  false;
            isInDefaultState &= partHeaderList->getScorePart()->getPartAbbreviation()->getValue().getValue() ==  "";
            isInDefaultState &= partHeaderList->getScorePart()->getPartName()->getValue().getValue() == "";
            isInDefaultState &= partHeaderList->getScorePart()->getHasPartNameDisplay() ==  false;
            
            return ! isInDefaultState;
        }
        
        
        ScorePartPtr makePartDeclaration( const PartParams& params )
        {
            auto sp = makeScorePart();
            sp->getAttributes()->id = XsID( params.uniqueId );
            sp->getPartName()->setValue( XsString( params.name ) );
            
            if( params.abbreviation.size() > 0 )
            {
                sp->setHasPartAbbreviation( true );
                sp->getPartAbbreviation()->setValue( XsString( params.abbreviation ) );
            }
            
            if( params.displayName.size() > 0 )
            {
                sp->setHasPartNameDisplay( true );
                auto dtoat = makeDisplayTextOrAccidentalText();
                dtoat->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
                dtoat->getDisplayText()->setValue( XsString( params.displayName ) );
                sp->getPartNameDisplay()->addDisplayTextOrAccidentalText( dtoat );
            }
            
            if( params.displayAbbreviation.size() > 0 )
            {
                sp->setHasPartAbbreviation( true );
                auto dtoat = makeDisplayTextOrAccidentalText();
                dtoat->setChoice( DisplayTextOrAccidentalText::Choice::displayText );
                dtoat->getDisplayText()->setValue( XsString( params.displayAbbreviation ) );
                sp->getPartNameDisplay()->addDisplayTextOrAccidentalText( dtoat );
            }
            addScoreInstrumentToPart( sp, params );
            addVirtualInstrumentToScoreInstrument( sp, params );
            addMidiInstrumentToPart( sp, params );
            return sp;
        }
        
        
        void addScoreInstrumentToPart(
            ScorePartPtr& part,
            const PartParams& params )
        {
            if( params.instrumentUniqueId.size() == 0 )
            {
                return;
            }
            auto scoreInstrument = makeScoreInstrument();
            scoreInstrument->getAttributes()->id = XsID( params.instrumentUniqueId );
            scoreInstrument->getInstrumentName()->setValue( XsString( params.instrumentName ) );
            scoreInstrument->setHasInstrumentAbbreviation( params.instrumentAbbreviation.size() > 0 );
            scoreInstrument->getInstrumentAbbreviation()->setValue( params.instrumentAbbreviation );
            part->addScoreInstrument( scoreInstrument );
        }
        
        
        void addVirtualInstrumentToScoreInstrument(
            ScorePartPtr& part,
            const PartParams& params )
        {
            if( params.instrumentUniqueId.size() == 0 ||
                part->getScoreInstrumentSet().size() == 0 )
            {
                return;
            }
            auto scoreInstrument = *( part->getScoreInstrumentSet().cbegin() );
            auto virtualInstrument = makeVirtualInstrument();
            virtualInstrument->setHasVirtualName( params.virtualInstrument.size() > 0 );
            virtualInstrument->getVirtualName()->setValue( XsString( params.virtualInstrument ) );
            virtualInstrument->setHasVirtualLibrary( params.virtualLibrary.size() > 0 );
            virtualInstrument->getVirtualLibrary()->setValue( XsString( params.virtualLibrary ) );
            scoreInstrument->setHasVirtualInstrument( true );
            scoreInstrument->setVirtualInstrument( virtualInstrument );
        }
        
        
        void addMidiInstrumentToPart( ScorePartPtr& part, const PartParams& params )
        {
            if( params.instrumentUniqueId.size() == 0 )
            {
                return;
            }
            if( params.midiBank < 0 &&
                params.midiChannel < 0 &&
                params.midiPan < -180 &&
                params.midiProgram < 0 &&
                params.midiVolume < 0 )
            {
                return;
            }
            auto midiStuff = makeMidiDeviceInstrumentGroup();
            part->addMidiDeviceInstrumentGroup( midiStuff );
            
            if( params.midiBank > 0 )
            {
                midiStuff->setHasMidiDevice( true );
                StringType bank( "Bank " );
                bank = bank + std::to_string( params.midiBank );
                midiStuff->getMidiDevice()->setValue( XsString( bank ) );
            }
            midiStuff->setHasMidiInstrument( true );
            auto midiInstrument = midiStuff->getMidiInstrument();
            midiInstrument->getAttributes()->id = XsID( params.instrumentUniqueId );
            
            if( params.midiChannel > 0 )
            {
                midiInstrument->setHasMidiChannel( true );
                midiInstrument->getMidiChannel()->setValue( Midi16( params.midiChannel ) );
            }
            
            if( params.midiProgram > 0 )
            {
                midiInstrument->setHasMidiProgram( true );
                midiInstrument->getMidiProgram()->setValue( Midi128( params.midiProgram ) );
            }
            
            if( params.midiVolume >= 0 )
            {
                midiInstrument->setHasVolume( true );
                midiInstrument->getVolume()->setValue( Percent( params.midiVolume ) );
            }
            
            if( params.midiVolume >= -180 )
            {
                midiInstrument->setHasPan( true );
                midiInstrument->getPan()->setValue( RotationDegrees( params.midiVolume ) );
            }
            
            if( params.midiName.size() > 0 )
            {
                midiInstrument->setHasMidiName( true );
                midiInstrument->getMidiName()->setValue( XsString( params.midiName ) );
            }
        }
        
        PartwisePartPtr makePartForMusicData( const PartParams& params )
        {
            auto part = makePartwisePart();
            part->getAttributes()->id = XsID( params.uniqueId );
            return part;
        }
        
        
        PartwisePartPtr addInitialPartToDocument( const MxDoc& doc, const PartParams& params )
        {
            auto partDeclaration = makePartDeclaration( params );
            auto partData = makePartForMusicData( params );
            auto headerList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
            headerList->setScorePart( partDeclaration );
            doc->getScorePartwise()->addPartwisePart( partData );
            auto partToReplace = doc->getScorePartwise()->getPartwisePartSet().cbegin();
            doc->getScorePartwise()->removePartwisePart( partToReplace );
            return *doc->getScorePartwise()->getPartwisePartSet().cbegin();
        }
        
        
        PartwisePartPtr addSubsequentPartToDocument( const MxDoc& doc, const PartParams& params )
        {
            auto partDeclaration = makePartDeclaration( params );
            auto partData = makePartForMusicData( params );
            auto headerList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
            auto itemToAdd = makePartGroupOrScorePart();
            itemToAdd->setChoice( PartGroupOrScorePart::Choice::scorePart );
            itemToAdd->setScorePart( partDeclaration );
            headerList->addPartGroupOrScorePart( itemToAdd );
            doc->getScorePartwise()->addPartwisePart( partData );
            return partData;
        }
        
        
        MusicDataChoicePtr createOrRetrieveMeasureProperties( const PartwiseMeasurePtr& measure )
        {
            auto iter = measure->getMusicDataGroup()->getMusicDataChoiceSet().cbegin();
            auto endIter = measure->getMusicDataGroup()->getMusicDataChoiceSet().cend();
            for ( ; iter != endIter; ++iter )
            {
                if( (*iter)->getChoice() == MusicDataChoice::Choice::properties )
                {
                    return *iter;
                }
            }

            auto musicDataChoice = makeMusicDataChoice();
            musicDataChoice->setChoice( MusicDataChoice::Choice::properties );
            measure->getMusicDataGroup()->addMusicDataChoice( musicDataChoice );
            return musicDataChoice;
        }
        
        
        MusicDataChoicePtr createEmptyNote( NoteChoice::Choice choice )
        {
            auto musicDataChoice = makeMusicDataChoice();
            musicDataChoice->setChoice( MusicDataChoice::Choice::note );
            musicDataChoice->getNote()->getNoteChoice()->setChoice( choice );
            return musicDataChoice;
        }
        
        
        FullNoteGroupPtr findFullNoteGroup( const NotePtr& noteElement )
        {
            FullNoteGroupPtr fullNoteGroup;
            switch ( noteElement->getNoteChoice()->getChoice() )
            {
                case NoteChoice::Choice::normal:
                {
                    auto normalNoteGroup = noteElement->getNoteChoice()->getNormalNoteGroup();
                    fullNoteGroup = normalNoteGroup->getFullNoteGroup();
                    break;
                }
                case NoteChoice::Choice::cue:
                {
                    auto cueNoteGroup = noteElement->getNoteChoice()->getCueNoteGroup();
                    fullNoteGroup = cueNoteGroup->getFullNoteGroup();
                    break;
                }
                case NoteChoice::Choice::grace:
                {
                    auto graceNoteGroup = noteElement->getNoteChoice()->getGraceNoteGroup();
                    fullNoteGroup = graceNoteGroup->getFullNoteGroup();
                    break;
                }
                default:
                    fullNoteGroup = nullptr;
            }
            return fullNoteGroup;
        }
        
        
        void setNoteStep( const MusicDataChoicePtr& mdc, int step )
        {
            StepEnum stepVal = StepEnum::c;
            switch( step )
            {
                case 0:
                    stepVal = StepEnum::c;
                    break;
                case 1:
                    stepVal = StepEnum::d;
                    break;
                case 2:
                    stepVal = StepEnum::e;
                    break;
                case 3:
                    stepVal = StepEnum::f;
                    break;
                case 4:
                    stepVal = StepEnum::g;
                    break;
                case 5:
                    stepVal = StepEnum::a;
                    break;
                case 6:
                    stepVal = StepEnum::b;
                    break;
                default:
                    stepVal = StepEnum::c;
                    break;
            }
            auto noteElement = mdc->getNote();
            FullNoteGroupPtr fullNoteGroup = findFullNoteGroup( noteElement );
            auto pitch = fullNoteGroup->getFullNoteTypeChoice()->getPitch();
            pitch->getStep()->setValue( stepVal );
        }
        
        
        void setNoteAlter( const MusicDataChoicePtr& mdc, int alter )
        {
            auto noteElement = mdc->getNote();
            FullNoteGroupPtr fullNoteGroup = findFullNoteGroup( noteElement );
            fullNoteGroup->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
            auto pitch = fullNoteGroup->getFullNoteTypeChoice()->getPitch();
            pitch->setHasAlter( true );
            pitch->getAlter()->setValue( Semitones{ static_cast<DecimalType>(alter) } );
        }
        
        
        void setNoteOctave( const MusicDataChoicePtr& mdc, int octave )
        {
            auto noteElement = mdc->getNote();
            FullNoteGroupPtr fullNoteGroup = findFullNoteGroup( noteElement );
            fullNoteGroup->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
            auto pitch = fullNoteGroup->getFullNoteTypeChoice()->getPitch();
            pitch->getOctave()->setValue( OctaveValue( octave ) );
        }
        
        
        void setNoteStaffNumber( const MusicDataChoicePtr& mdc, int staffNumber )
        {
            auto noteElement = mdc->getNote();
            noteElement->setHasStaff( true );
            noteElement->getStaff()->setValue( PositiveInteger{ staffNumber } );
        }
        
        
        void setNoteVoiceNumber( const MusicDataChoicePtr& mdc, int voiceNumber )
        {
            auto noteElement = mdc->getNote();
            noteElement->getEditorialVoiceGroup()->setHasVoice( true );
            noteElement->getEditorialVoiceGroup()->getVoice()->setValue( XsString( std::to_string( voiceNumber ) ) );
        }
        
        
        void setStemDirection( const MusicDataChoicePtr& mdc, mx::utility::UpDown stemDirection )
        {
            if( stemDirection == UpDown::unspecified )
            {
                mdc->getNote()->setHasStem( false );
                return;
            }
            
            mdc->getNote()->setHasStem( false );
            auto value = StemValue::up;

            if( stemDirection == UpDown::down )
            {
                value = StemValue::down;
            }
            
            mdc->getNote()->setHasStem( true );
            mdc->getNote()->getStem()->setValue( value );
        }
        
        
        void setNoteDurationType( const MusicDataChoicePtr& mdc, NoteTypeValue durationType )
        {
            auto noteElement = mdc->getNote();
            noteElement->setHasType( true );
            noteElement->getType()->setValue( durationType );
        }
        
        
        void setNoteDurationInt( const MusicDataChoicePtr& mdc, int noteDuration )
        {
            auto divisionsValue = PositiveDivisionsValue( static_cast<DecimalType>( noteDuration ) );
            auto noteChoice = mdc->getNote()->getNoteChoice();
            switch ( noteChoice->getChoice() )
            {
                case NoteChoice::Choice::normal:
                {
                    noteChoice->getNormalNoteGroup()->getDuration()->setValue( divisionsValue );
                    return;
                }
                case NoteChoice::Choice::cue:
                {
                    noteChoice->getCueNoteGroup()->getDuration()->setValue( divisionsValue );
                    return;
                }
                case NoteChoice::Choice::grace:
                {
                    // do nothing, grace notes do not have duration element
                    return;
                }
                default:
                    return;
            }
        }
        
        
        void setNoteDurationDots( const MusicDataChoicePtr& mdc, int dots )
        {
            for( int i = 0; i < dots; ++i )
            {
                mdc->getNote()->addDot( makeDot() );
            }
        }
        
        
        void setNoteBeams( const MusicDataChoicePtr& mdc, const std::vector<BeamValue>& beams )
        {
            int i = 1;
            for( auto it = beams.cbegin(); it != beams.cend(); ++it, ++i )
            {
                auto beam = makeBeam();
                beam->getAttributes()->hasNumber = true;
                beam->getAttributes()->number = BeamLevel( i );
                beam->setValue( *it );
                mdc->getNote()->addBeam( beam );
            }
        }
        
        
        void setNoteAccidental( const MusicDataChoicePtr& mdc, int alter )
        {
            AccidentalValue accidental = AccidentalValue::natural;
            if( alter <= -2 )
            {
                accidental = AccidentalValue::flatFlat;
            }
            else if ( alter == -1 )
            {
                accidental = AccidentalValue::flat;
            }
            else if ( alter == 1 )
            {
                accidental = AccidentalValue::sharp;
            }
            else if ( alter >= 2 )
            {
                accidental = AccidentalValue::doubleSharp;
            }
            mdc->getNote()->setHasAccidental( true );
            mdc->getNote()->getAccidental()->setValue( accidental );
        }
        

        void setNoteIsRest( const MusicDataChoicePtr& mdc )
        {
            // TODO - what if the "current" choice is not the NormalNoteGroup?
            mdc->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::rest );
        }


        void setNoteIsChord( const MusicDataChoicePtr& mdc )
        {
            // TODO - what if the "current" choice is not the NormalNoteGroup?
            mdc->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->setHasChord( true );
        }


        void setNoteIsGrace( const MusicDataChoicePtr& mdc )
        {
            // TODO - what if the "current" choice has data that should be moved to
            // the grace choice?  Should it be moved?
            mdc->getNote()->getNoteChoice()->setChoice( NoteChoice::Choice::grace );
        }
        
        
        void setNoteIsCue( const MusicDataChoicePtr& mdc )
        {
            // TODO - what if the "current" choice has data that should be moved to
            // the grace choice?  Should it be moved?
            mdc->getNote()->getNoteChoice()->setChoice( NoteChoice::Choice::cue );
        }
        

        MusicDataChoicePtr getOrAddMeasureProperties( const PartwiseMeasurePtr& measure )
        {
            for ( auto it = measure->getMusicDataGroup()->getMusicDataChoiceSet().cbegin();
                  it != measure->getMusicDataGroup()->getMusicDataChoiceSet().cend(); ++it )
            {
                if ( (*it)->getChoice() == MusicDataChoice::Choice::properties )
                {
                    return (*it);
                }
            }
            auto mdc = makeMusicDataChoice();
            mdc->setChoice( MusicDataChoice::Choice::properties );
            measure->getMusicDataGroup()->addMusicDataChoice( mdc );
            return mdc;
        }
        
        
    } // namespace utility

} // namespace mx
