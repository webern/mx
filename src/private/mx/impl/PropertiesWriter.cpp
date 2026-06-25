// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PropertiesWriter.h"
#include "mx/core/Decimal.h"
#include "mx/core/OneOrMore.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/AttributesChoice.h"
#include "mx/core/generated/Cancel.h"
#include "mx/core/generated/Clef.h"
#include "mx/core/generated/ClefGroup.h"
#include "mx/core/generated/Fifths.h"
#include "mx/core/generated/Key.h"
#include "mx/core/generated/KeyAccidental.h"
#include "mx/core/generated/KeyChoice.h"
#include "mx/core/generated/Mode.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NonTraditionalKeyGroup.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/PositiveDivisions.h"
#include "mx/core/generated/Semitones.h"
#include "mx/core/generated/StaffDetails.h"
#include "mx/core/generated/StaffDetailsGroup.h"
#include "mx/core/generated/StaffLinePosition.h"
#include "mx/core/generated/StaffNumber.h"
#include "mx/core/generated/Time.h"
#include "mx/core/generated/TimeChoice.h"
#include "mx/core/generated/TimeChoiceGroup.h"
#include "mx/core/generated/TimeSignatureGroup.h"
#include "mx/core/generated/TimeSymbol.h"
#include "mx/core/generated/TraditionalKeyGroup.h"
#include "mx/core/generated/Transpose.h"
#include "mx/impl/Converter.h"

namespace mx
{
namespace impl
{

PropertiesWriter::PropertiesWriter(core::PartwiseMeasure &inPartwiseMeasure)
    : myAttributes{}, myHasContent{false}, myPartwiseMeasure{inPartwiseMeasure}
{
}

void PropertiesWriter::clearBuffer()
{
    allocate();
}

void PropertiesWriter::flushBuffer()
{
    if (!isPropertiesEmpty())
    {
        myPartwiseMeasure.addMusicData(core::MusicDataChoice::attributes(myAttributes));
    }
    allocate();
}

bool PropertiesWriter::isPropertiesEmpty()
{
    return !myHasContent;
}

void PropertiesWriter::writeDivisions(int value)
{
    myAttributes.setDivisions(core::PositiveDivisions{core::Decimal{static_cast<double>(value)}});
    myHasContent = true;
}

void PropertiesWriter::writeKey(int staffIndex, const api::KeyData &inKeyData)
{
    core::Key key{};

    if (staffIndex >= 0)
    {
        key.setNumber(core::StaffNumber{staffIndex + 1});
    }

    if (inKeyData.nonTraditional.empty())
    {
        writeTraditionalKey(inKeyData, key);
    }
    else
    {
        writeNonTraditionalKey(inKeyData, key);
    }

    myAttributes.addKey(key);
    myHasContent = true;
}

void PropertiesWriter::writeNonTraditionalKey(const api::KeyData &inKeyData, core::Key &ioKey)
{
    Converter converter;
    std::vector<core::NonTraditionalKeyGroup> groups;
    for (const auto &keyComponent : inKeyData.nonTraditional)
    {
        core::NonTraditionalKeyGroup nt{};
        if (keyComponent.accidental != api::Accidental::none)
        {
            core::KeyAccidental ka{};
            ka.setValue(converter.convert(keyComponent.accidental));
            nt.setKeyAccidental(ka);
        }
        const auto isUnknown = keyComponent.step == api::Step::unspecified;
        const auto step = isUnknown ? api::Step::c : keyComponent.step;
        nt.setKeyStep(converter.convert(step));
        const auto alter = Converter::convertToAlter(keyComponent.alter, keyComponent.cents);
        nt.setKeyAlter(core::Semitones{core::Decimal{alter}});
        groups.push_back(std::move(nt));
    }
    ioKey.setChoice(core::KeyChoice::nonTraditionalKey(std::move(groups)));
}

void PropertiesWriter::writeTraditionalKey(const api::KeyData &inKeyData, core::Key &ioKey)
{
    core::TraditionalKeyGroup tkg{};
    tkg.setFifths(core::Fifths{inKeyData.fifths});

    if (inKeyData.cancel != 0)
    {
        core::Cancel cancel{};
        cancel.setValue(core::Fifths{inKeyData.cancel});
        tkg.setCancel(cancel);
    }

    if (inKeyData.mode == api::KeyMode::major || inKeyData.mode == api::KeyMode::minor)
    {
        const auto modeStr = (inKeyData.mode == api::KeyMode::major) ? "major" : "minor";
        tkg.setMode(core::Mode{modeStr});
    }

    ioKey.setChoice(core::KeyChoice::traditionalKey(tkg));
}

void PropertiesWriter::writeTime(const api::TimeSignatureData &value)
{
    core::Time time{};

    core::TimeSignatureGroup tsg{};
    tsg.setBeats(value.beats);
    tsg.setBeatType(value.beatType);

    core::TimeChoiceGroup tcg{};
    tcg.setTimeSignature(core::OneOrMore<core::TimeSignatureGroup>{tsg});

    time.setChoice(core::TimeChoice::group(tcg));

    const auto symbol = value.symbol;
    if (symbol != api::TimeSignatureSymbol::unspecified)
    {
        if (symbol == api::TimeSignatureSymbol::common)
        {
            time.setSymbol(core::TimeSymbol::common());
        }
        else if (symbol == api::TimeSignatureSymbol::cut)
        {
            time.setSymbol(core::TimeSymbol::cut());
        }
        else if (symbol == api::TimeSignatureSymbol::singleNumber)
        {
            time.setSymbol(core::TimeSymbol::singleNumber());
        }
    }

    Converter converter;
    if (value.display != api::Bool::unspecified)
    {
        time.setPrintObject(converter.convert(value.display));
    }

    myAttributes.addTime(time);
    myHasContent = true;
}

void PropertiesWriter::writeNumStaves(int value)
{
    myAttributes.setStaves(value);
    myHasContent = true;
}

void PropertiesWriter::writeStaffDetails(int staffIndex, int staffLines)
{
    core::StaffDetails staffDetails{};

    if (staffIndex >= 0)
    {
        staffDetails.setNumber(core::StaffNumber{staffIndex + 1});
    }

    core::StaffDetailsGroup sdg{};
    sdg.setStaffLines(staffLines);
    staffDetails.setGroup(sdg);

    myAttributes.addStaffDetails(staffDetails);
    myHasContent = true;
}

void PropertiesWriter::writeClef(int staffIndex, const api::ClefData &inClefData)
{
    core::Clef mxClef{};

    if (staffIndex >= 0)
    {
        mxClef.setNumber(core::StaffNumber{staffIndex + 1});
    }

    Converter converter;
    core::ClefGroup cg{};
    cg.setSign(converter.convert(inClefData.symbol));

    if (inClefData.isLineSpecified)
    {
        cg.setLine(core::StaffLinePosition{inClefData.line});
    }

    if (inClefData.octaveChange != 0)
    {
        cg.setClefOctaveChange(inClefData.octaveChange);
    }

    mxClef.setClef(cg);
    myAttributes.addClef(mxClef);
    myHasContent = true;
}

void PropertiesWriter::writeTranspose(const api::TransposeData &inTransposeData)
{
    auto xpose = Converter::convertToTranspose(inTransposeData);
    auto vec =
        myAttributes.choice().isTranspose() ? myAttributes.choice().asTranspose() : std::vector<core::Transpose>{};
    vec.push_back(std::move(xpose));
    myAttributes.setChoice(core::AttributesChoice::transpose(std::move(vec)));
    myHasContent = true;
}

void PropertiesWriter::allocate()
{
    myAttributes = core::Attributes{};
    myHasContent = false;
}

} // namespace impl
} // namespace mx
