// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/NoteData.h"
#include "mx/impl/MeasureFunctions.h"
#include "mx/impl/TimeFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/FiguredBass.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Barline.h"
#include "mx/core/elements/Grouping.h"
#include "mx/core/elements/Link.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/Rest.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Unpitched.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include "mx/core/elements/DisplayStep.h"
#include "mx/core/elements/Step.h"
#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Alter.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/Type.h"

#include <string>

namespace mx
{
    namespace impl
    {
        api::Step converCoreStepToApiStep( core::StepEnum coreStep )
        {
            switch ( coreStep )
            {
                case core::StepEnum::c:
                    return api::Step::c;
                case core::StepEnum::d:
                    return api::Step::d;
                case core::StepEnum::e:
                    return api::Step::e;
                case core::StepEnum::f:
                    return api::Step::f;
                case core::StepEnum::g:
                    return api::Step::g;
                case core::StepEnum::a:
                    return api::Step::a;
                case core::StepEnum::b:
                    return api::Step::b;
                default:
                    break;
            }
            MX_BUG;
        }
        
        api::DurationName converCoreDurationEnumToApiDurationEnum( core::NoteTypeValue noteTypeValue )
        {
            switch ( noteTypeValue )
            {
                case core::NoteTypeValue::oneThousandTwentyFourth: return api::DurationName::dur1024th;
                case core::NoteTypeValue::fiveHundredTwelfth: return api::DurationName::dur512th;
                case core::NoteTypeValue::twoHundredFifthySixth: return api::DurationName::dur256th;
                case core::NoteTypeValue::oneHundredTwentyEighth: return api::DurationName::dur128th;
                case core::NoteTypeValue::sixtyFourth: return api::DurationName::dur64th;
                case core::NoteTypeValue::thirtySecond: return api::DurationName::dur32nd;
                case core::NoteTypeValue::sixteenth: return api::DurationName::dur16th;
                case core::NoteTypeValue::eighth: return api::DurationName::eighth;
                case core::NoteTypeValue::quarter: return api::DurationName::quarter;
                case core::NoteTypeValue::half: return api::DurationName::half;
                case core::NoteTypeValue::whole: return api::DurationName::whole;
                case core::NoteTypeValue::breve: return api::DurationName::breve;
                case core::NoteTypeValue::long_: return api::DurationName::longa;
                case core::NoteTypeValue::maxima: return api::DurationName::maxima;
                default:
                    break;
            }
            return api::DurationName::unspecified;
        }
        
		StaffIndexMeasureMap parseMeasure( const core::PartwiseMeasure& inMxMeasure, MeasureCursor& cursor, bool isFirstMeasure )
        {
            cursor.reset();
            StaffIndexMeasureMap outMap;
            
            if( isFirstMeasure )
            {
                cursor.timeSignature.isImplicit = false;
                
            }
            
            auto startingCursor = cursor;
            bool isTimeSignatureFound = false;
            
            const auto& mdcSet = inMxMeasure.getMusicDataGroup()->getMusicDataChoiceSet();
            
            for( const auto& mdc : mdcSet )
            {
                if( !isTimeSignatureFound )
                {
                    isTimeSignatureFound = findAndFillTimeSignature( *mdc, cursor.timeSignature );
                    if( isFirstMeasure )
                    {
                        cursor.timeSignature.isImplicit = false;
                    }
                    else
                    {
                        if( ( cursor.timeSignature.beats != startingCursor.timeSignature.beats ) ||
                           ( cursor.timeSignature.beatType != startingCursor.timeSignature.beatType ) ||
                           ( cursor.timeSignature.symbol != startingCursor.timeSignature.symbol ) ||
                           ( cursor.timeSignature.isPrintObjectYes ) )
                            
                        {
                            cursor.timeSignature.isImplicit = false;
                        }
                        else
                        {
                            cursor.timeSignature.isImplicit = true;
                        }
                    }
                }
                
                const auto choice = mdc->getChoice();
                switch ( choice )
                {
                    case core::MusicDataChoice::Choice::note:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& mxNote = *mdc->getNote();
                        
                        if( mxNote.getHasStaff() )
                        {
                            cursor.currentVoiceIndex = mxNote.getStaff()->getValue().getValue() - 1;
                        }
                        
                        const auto& ncObject = *mxNote.getNoteChoice();
                        const auto& ncChoice = ncObject.getChoice();
                        core::FullNoteGroupPtr fullNoteGroup;
                        int durationValue = cursor.currentTicksPerQuarter;
                        int tieCount = 0;
                        bool isCue = false;
                        bool isGrace = false;
                        
                        switch ( ncChoice )
                        {
                            case core::NoteChoice::Choice::normal:
                            {
                                const auto& noteGuts = *ncObject.getNormalNoteGroup();
                                fullNoteGroup = noteGuts.getFullNoteGroup();
                                tieCount = noteGuts.getTieSet().size();
                                durationValue = cursor.convertToGlobalTickScale( static_cast<int>( std::ceil( noteGuts.getDuration()->getValue().getValue() - 0.5 ) ) );
                                break;
                            }
                            case core::NoteChoice::Choice::grace:
                            {
                                isGrace = true;
                                const auto& noteGuts = *ncObject.getGraceNoteGroup();
                                fullNoteGroup = noteGuts.getFullNoteGroup();
                                tieCount = noteGuts.getTieSet().size();
                                durationValue = 0;
                                break;
                            }
                            case core::NoteChoice::Choice::cue:
                            {
                                isCue = true;
                                const auto& noteGuts = *ncObject.getCueNoteGroup();
                                fullNoteGroup = noteGuts.getFullNoteGroup();
                                durationValue = cursor.convertToGlobalTickScale( static_cast<int>( std::ceil( noteGuts.getDuration()->getValue().getValue() - 0.5 ) ) );
                                tieCount = 0;
                                break;
                            }
                            default:
                                MX_BUG;
                                break;
                        }
                        bool isChord = fullNoteGroup->getHasChord();
                        const auto& fntcObject = *fullNoteGroup->getFullNoteTypeChoice();
                        const auto& fntcChoice = fntcObject.getChoice();
                        bool isRest = false;
                        bool isUnpitched = false;
                        auto step = core::StepEnum::c;
                        int octave = 4;
                        int alter = 0;
                        bool isDisplayStepOctaveSpecified = false;
                        
                        switch ( fntcChoice )
                        {
                            case core::FullNoteTypeChoice::Choice::rest:
                            {
                                isRest = true;
                                const auto& rest = *fntcObject.getRest();
                                const auto& stepOctave = *rest.getDisplayStepOctaveGroup();

                                if( rest.getHasDisplayStepOctaveGroup() )
                                {
                                    isDisplayStepOctaveSpecified = true;
                                    step = stepOctave.getDisplayStep()->getValue();
                                    octave = stepOctave.getDisplayOctave()->getValue().getValue();
                                }
                                
                                alter = 0;
                                break;
                            }
                            case core::FullNoteTypeChoice::Choice::unpitched:
                            {
                                isUnpitched = true;
                                const auto& unpitched = *fntcObject.getUnpitched();
                                const auto& stepOctave = *unpitched.getDisplayStepOctaveGroup();
                                
                                if( unpitched.getHasDisplayStepOctaveGroup() )
                                {
                                    isDisplayStepOctaveSpecified = true;
                                    step = stepOctave.getDisplayStep()->getValue();
                                    octave = stepOctave.getDisplayOctave()->getValue().getValue();
                                }

                                alter = 0;
                                break;
                            }
                            case core::FullNoteTypeChoice::Choice::pitch:
                            {
                                const auto& pitch = *fntcObject.getPitch();
                                step = pitch.getStep()->getValue();
                                octave = pitch.getOctave()->getValue().getValue();
                                alter = static_cast<int>( std::ceil( pitch.getAlter()->getValue().getValue() - 0.5 ) );
                                break;
                            }
                                
                            default:
                                break;
                        }
                        
                        int userRequestedVoiceNumber = -1;
                        if( mxNote.getEditorialVoiceGroup()->getHasVoice() )
                        {
                            try
                            {
                                userRequestedVoiceNumber = std::stoi( mxNote.getEditorialVoiceGroup()->getVoice()->getValue().getValue() );
                            }
                            catch ( ... )
                            {
                                
                            }
                        }

                        cursor.currentStaffIndex = mxNote.getHasStaff() ? mxNote.getStaff()->getValue().getValue() - 1 : 0;
                        
                        auto mxNoteTypeValue = mxNote.getType()->getValue();
                        
                        auto noteData = api::NoteData{};
                        noteData.isRest = isRest;
                        
                        noteData.noteType = api::NoteType::normal;
                        
                        if( isCue )
                        {
                            noteData.noteType = api::NoteType::cue;
                        }
                        
                        if( isGrace )
                        {
                            noteData.noteType = api::NoteType::grace;
                        }
                        
                        noteData.durationTicks = durationValue;
                        noteData.startPosition = cursor.currentPositionTicks;
                        noteData.staffIndex = cursor.currentStaffIndex;
                        noteData.userRequestedVoiceNumber = userRequestedVoiceNumber;
                        noteData.step = converCoreStepToApiStep( step );
                        noteData.alter = alter;
                        noteData.octave = octave;
                        noteData.durationName = ( mxNote.getHasType() ? converCoreDurationEnumToApiDurationEnum( mxNoteTypeValue ) : api::DurationName::unspecified );
                        auto& outMeasure = outMap[cursor.currentStaffIndex];
                        outMeasure.voices[cursor.currentVoiceIndex].notes.emplace_back( std::move( noteData ) );
                        
                        if( !isChord )
                        {
                            cursor.currentPositionTicks += durationValue;
                        }
                        
                        break;
                    }
                    case core::MusicDataChoice::Choice::backup:
                    {
                        MX_THROW( "i am not working" );
                        if( !cursor.isBackupInProgress )
                        {
                            ++cursor.currentVoiceIndex;
                        }
                        cursor.isBackupInProgress = true;
                        const auto& item = *mdc->getBackup();
                        const int backupAmount = static_cast<int>( std::ceil( item.getDuration()->getValue().getValue() - 0.5 ) );
                        cursor.currentPositionTicks -= backupAmount;
                        break;
                    }
                    case core::MusicDataChoice::Choice::forward:
                    {
                        
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getForward();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::direction:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getDirection();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::properties:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getProperties();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::harmony:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getHarmony();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::figuredBass:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getFiguredBass();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::print:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getPrint();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::sound:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getSound();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::barline:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getBarline();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::grouping:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getGrouping();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::link:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getLink();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    case core::MusicDataChoice::Choice::bookmark:
                    {
                        cursor.isBackupInProgress = false;
                        const auto& item = *mdc->getBookmark();
                        std::cout << item.getElementName() << " is not supported" << std::endl;
                        break;
                    }
                    default:
                    {
                        MX_THROW( "unsupported MusicDataChoice::Choice value" );
                    }
                }
            }
            
            for( int i = 0; i < cursor.numStaves; ++i )
            {
                outMap[i].timeSignature = cursor.timeSignature;
            }
            
            return outMap;
        }
    }
}
