#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>

// The purpose of this executable is to demonstrate whether or
// not there is any leakage from `mx::impl` or `mx::api` headers
// into `mx::api`. i.e. we want `mx/api` to be our public
// include directory, and if it's reaching out to any other
// directories then we want to fix it. Compilining this
// example should fail if we have such header leakage

#include "mx/api/ApiCommon.h"
#include "mx/api/ApiEquality.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/BarlineData.h"
#include "mx/api/ChordData.h"
#include "mx/api/ClefData.h"
#include "mx/api/ColorData.h"
#include "mx/api/CurveData.h"
#include "mx/api/DirectionData.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/DurationData.h"
#include "mx/api/EncodingData.h"
#include "mx/api/FontData.h"
#include "mx/api/KeyData.h"
#include "mx/api/SetupData.h"
#include "mx/api/LineData.h"
#include "mx/api/LyricData.h"
#include "mx/api/MarkData.h"
#include "mx/api/MeasureData.h"
#include "mx/api/MeasureLocation.h"
#include "mx/api/MiscData.h"
#include "mx/api/NoteAttachmentData.h"
#include "mx/api/NoteData.h"
#include "mx/api/OttavaData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"
#include "mx/api/PartGroupData.h"
#include "mx/api/PitchData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/ScoreData.h"
#include "mx/api/SoundID.h"
#include "mx/api/SpannerData.h"
#include "mx/api/StaffData.h"
#include "mx/api/SystemData.h"
#include "mx/api/TempoData.h"
#include "mx/api/TimeSignatureData.h"
#include "mx/api/TupletData.h"
#include "mx/api/Version.h"
#include "mx/api/VoiceData.h"
#include "mx/api/WedgeData.h"
#include "mx/api/WordsData.h"

int main(int argc, const char * argv[])
{
    using namespace mx::api;
    const auto qticks = 4;

    // create a score
    auto score = ScoreData{};
    score.workTitle = "nothing";
    std::stringstream ss;
    ss << score.workTitle;

    if( ss.str().empty() )
    {
        return 1;
    }

    return 0;
}