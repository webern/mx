// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include <iostream>

#include "mx/api/DocumentManager.h"

using namespace mx::api;

constexpr const char* const fileName =
// "./Resources/lysuite/ly02c_Rests_MultiMeasureRests.xml";
// "./Resources/mjbsuite/freezing.xml";
"./Resources/mjbsuite/FreezingSmall.xml";
// "./Resources/ksuite/k011a_Tuplets.xml";
// "./Resources/lysuite/ly33a_Spanners.xml";
// "./Resources/ksuite/k012a_Tempo_Markings.xml";
// "./Resources/lysuite/ly13a_KeySignatures.xml";
// "./Resources/ksuite/k001a_Articulations.xml";

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
