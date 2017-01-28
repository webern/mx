// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include <memory>
#include "mx/core/Enums.h"
#include "mx/core/Elements.h"

namespace mx
{
    namespace core
    {
        class Document;
        using DocumentPtr = std::shared_ptr<Document>;
    }
}

namespace mxtest
{
    mx::core::DocumentPtr createDocumentPartwise();
    void setPartwiseMeasureProperties( mx::core::PartwiseMeasurePtr& measure,
                                       int measureNumber,
                                       int divisions,
                                       int beats,
                                       int beatType );
    
    mx::core::MusicDataChoicePtr makeNote( mx::core::StepEnum step,
                                     int octave,
                                     mx::core::NoteTypeValue duration,
                                     int divisions );
    void addP1M1Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP1M2Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP1M3Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP2M1Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP2M2Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP2M3Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP3M1Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP3M2Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
    void addP3M3Data( const mx::core::MusicDataGroupPtr& musicDataGroup );
}

#endif
