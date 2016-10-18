// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include <iostream>

#include "mx/api/DocumentManager.h"

using namespace mx::api;

#define PERFORMANCE_LOOP

constexpr const char* const fileName =
// "./Resources/lysuite/ly01a_Pitches_Pitches.xml";
"./Resources/lysuite/ly02c_Rests_MultiMeasureRests.xml";
// "./Resources/mjbsuite/freezing.xml";
// "./Resources/mjbsuite/FreezingSmall.xml";
// "./Resources/mjbsuite/OctoberXML.xml";
// "./Resources/ksuite/k011a_Tuplets.xml";
// "./Resources/lysuite/ly33a_Spanners.xml";
// "./Resources/ksuite/k012a_Tempo_Markings.xml";
// "./Resources/lysuite/ly13a_KeySignatures.xml";
// "./Resources/ksuite/k001a_Articulations.xml";

#ifdef LOAD_ONE

int main(int argc, const char * argv[])
{
    auto& docMgr = DocumentManager::getInstance();
    auto docId = docMgr.createFromFile( fileName );
    auto scoreData = docMgr.getData( docId );
    docMgr.destroyDocument( docId );
    docId = docMgr.createFromScore( scoreData );
    docMgr.writeToFile( docId, "./output.xml" );
    docMgr.destroyDocument( docId );
    return 0;
}

#endif

#ifdef PERFORMANCE_LOOP

#include <fstream>
#include <sstream>

int main(int argc, const char * argv[])
{
    std::ifstream infile{ fileName };
    if( !infile.is_open() )
    {
        throw std::runtime_error{ "blarf" };
    }

    std::string str((std::istreambuf_iterator<char>(infile)),
                    std::istreambuf_iterator<char>());
    
    
    auto& docMgr = DocumentManager::getInstance();
    mx::api::ScoreData scoreData;
    
    for( int i = 0; i < 1000000; ++i )
    {
        std::istringstream iss{ str };
        auto docId = docMgr.createFromStream( iss );
        scoreData = docMgr.getData( docId );
        docMgr.destroyDocument( docId );
    }
    
    //docId = docMgr.createFromScore( scoreData );
    //docMgr.writeToFile( docId, "./output.xml" );
    //docMgr.destroyDocument( docId );
    return 0;
}

#endif
