#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include <memory>
#include "mx/core/Enums.h"
#include "mx/core/Elements.h"

namespace mx
{
    namespace core
    {
        class DocumentTimewise;
        using DocumentTimewisePtr = std::shared_ptr<DocumentTimewise>;
    }
}

namespace MxTestHelpers
{
    mx::core::DocumentTimewisePtr createDocumentTimewise();
    void setTimewiseMeasureProperties( mx::core::TimewiseMeasurePtr& measure,
                                      int measureNumber,
                                      int divisions,
                                      int beats,
                                      int beatType );
    mx::core::MusicDataGroupPtr getConstructedMusicDataGroup( const std::string& beats );
    
//    MusicDataChoicePtr makeNote( mx::t::StepEnum step,
//                                       int octave,
//                                       mx::t::NoteTypeValue duration,
//                                       int divisions );
//    void addP1M1Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP1M2Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP1M3Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP2M1Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP2M2Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP2M3Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP3M1Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP3M2Data( const MusicDataGroupPtr& musicDataGroup );
//    void addP3M3Data( const MusicDataGroupPtr& musicDataGroup );
}

#endif
