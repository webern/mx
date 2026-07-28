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
#include "mx/core/generated/Interchangeable.h"
#include "mx/core/generated/Key.h"
#include "mx/core/generated/KeyAccidental.h"
#include "mx/core/generated/KeyChoice.h"
#include "mx/core/generated/MeasureStyle.h"
#include "mx/core/generated/MeasureStyleChoice.h"
#include "mx/core/generated/Mode.h"
#include "mx/core/generated/MultipleRest.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/NonTraditionalKeyGroup.h"
#include "mx/core/generated/PartSymbol.h"
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
#include "mx/core/generated/TimeRelation.h"
#include "mx/core/generated/TimeSeparator.h"
#include "mx/core/generated/TimeSignatureGroup.h"
#include "mx/core/generated/TimeSymbol.h"
#include "mx/core/generated/TraditionalKeyGroup.h"
#include "mx/core/generated/Transpose.h"
#include "mx/core/generated/YesNo.h"
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

void PropertiesWriter::writeMultipleRest(int measureCount, api::Bool useSymbols)
{
    core::MultipleRest multipleRest{};
    multipleRest.setValue(measureCount);
    if (useSymbols != api::Bool::unspecified)
    {
        multipleRest.setUseSymbols(useSymbols == api::Bool::yes ? core::YesNo::yes() : core::YesNo::no());
    }

    core::MeasureStyle measureStyle{};
    measureStyle.setChoice(core::MeasureStyleChoice::multipleRest(std::move(multipleRest)));
    myAttributes.addMeasureStyle(std::move(measureStyle));
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
        if (inKeyData.cancelLocation != api::CancelLocation::unspecified)
        {
            Converter converter;
            cancel.setLocation(converter.convert(inKeyData.cancelLocation));
        }
        tkg.setCancel(cancel);
    }

    if (inKeyData.mode == api::KeyMode::major || inKeyData.mode == api::KeyMode::minor)
    {
        const auto modeStr = (inKeyData.mode == api::KeyMode::major) ? "major" : "minor";
        tkg.setMode(core::Mode{modeStr});
    }

    ioKey.setChoice(core::KeyChoice::traditionalKey(tkg));
}

// converts a list of api fractions (a primary or interchangeable meter) to a core OneOrMore;
// an (invalid) empty meter falls back to a single 4/4 pair
static core::OneOrMore<core::TimeSignatureGroup> propertiesWriterTimeSignatureGroups(
    const std::vector<api::TimeFraction> &inFractions)
{
    std::vector<core::TimeSignatureGroup> groups;
    for (const auto &fraction : inFractions)
    {
        core::TimeSignatureGroup tsg{};
        tsg.setBeats(fraction.beats);
        tsg.setBeatType(fraction.beatType);
        groups.push_back(std::move(tsg));
    }
    if (groups.empty())
    {
        core::TimeSignatureGroup tsg{};
        tsg.setBeats("4");
        tsg.setBeatType("4");
        groups.push_back(std::move(tsg));
    }
    core::OneOrMore<core::TimeSignatureGroup> result{};
    result.setItems(std::move(groups));
    return result;
}

// builds the metered (group) TimeChoice from an api MeteredTimeSignature, and sets the meter's own
// symbol/separator on the <time> element
static void propertiesWriterSetMetered(core::Time &ioTime, const api::MeteredTimeSignature &inMetered,
                                       const Converter &converter)
{
    core::TimeChoiceGroup tcg{};
    tcg.setTimeSignature(propertiesWriterTimeSignatureGroups(inMetered.fractions));

    if (inMetered.interchangeable.has_value())
    {
        const auto &alternate = *inMetered.interchangeable;
        core::Interchangeable interchangeable{};
        interchangeable.setTimeSignature(propertiesWriterTimeSignatureGroups(alternate.fractions));
        if (alternate.relation != api::TimeRelation::unspecified)
        {
            interchangeable.setTimeRelation(converter.convert(alternate.relation));
        }
        if (alternate.symbol != api::ComplexTimeSymbol::unspecified)
        {
            interchangeable.setSymbol(converter.convert(alternate.symbol));
        }
        if (alternate.separator != api::TimeSeparator::unspecified)
        {
            interchangeable.setSeparator(converter.convert(alternate.separator));
        }
        tcg.setInterchangeable(std::move(interchangeable));
    }

    ioTime.setChoice(core::TimeChoice::group(std::move(tcg)));

    if (inMetered.symbol != api::ComplexTimeSymbol::unspecified)
    {
        ioTime.setSymbol(converter.convert(inMetered.symbol));
    }
    if (inMetered.separator != api::TimeSeparator::unspecified)
    {
        ioTime.setSeparator(converter.convert(inMetered.separator));
    }
}

void PropertiesWriter::writeTime(const api::TimeChoice &value, int staffIndex)
{
    Converter converter;
    core::Time time{};

    if (value.isSimple())
    {
        const auto &simple = value.simple();
        core::TimeChoiceGroup tcg{};
        std::vector<api::TimeFraction> fractions{simple.fraction};
        tcg.setTimeSignature(propertiesWriterTimeSignatureGroups(fractions));
        time.setChoice(core::TimeChoice::group(std::move(tcg)));
        if (simple.symbol != api::TimeSignatureSymbol::unspecified)
        {
            time.setSymbol(converter.convert(simple.symbol));
        }
    }
    else
    {
        const auto &complex = value.complex();
        if (complex.isSenzaMisura())
        {
            // the symbol attribute is not used with senza-misura
            time.setChoice(core::TimeChoice::senzaMisura(complex.senzaMisura()));
        }
        else
        {
            propertiesWriterSetMetered(time, complex.metered(), converter);
        }
    }

    if (value.display != api::Bool::unspecified)
    {
        time.setPrintObject(converter.convert(value.display));
    }

    if (staffIndex != api::INDEX_UNSPECIFIED)
    {
        time.setNumber(core::StaffNumber{staffIndex + 1});
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
    writeStaffDetails(staffIndex, staffLines, api::DOUBLE_UNSPECIFIED, api::DOUBLE_UNSPECIFIED);
}

void PropertiesWriter::writeStaffDetails(int staffIndex, int staffLines, double staffSize)
{
    writeStaffDetails(staffIndex, staffLines, staffSize, api::DOUBLE_UNSPECIFIED);
}

void PropertiesWriter::writeStaffDetails(int staffIndex, int staffLines, double staffSize, double staffScaling)
{
    core::StaffDetails staffDetails{};

    if (staffIndex >= 0)
    {
        staffDetails.setNumber(core::StaffNumber{staffIndex + 1});
    }

    if (staffLines >= 0)
    {
        core::StaffDetailsGroup sdg{};
        sdg.setStaffLines(staffLines);
        staffDetails.setGroup(sdg);
    }

    if (staffSize >= 0.0)
    {
        core::StaffSize size{};
        size.setValue(core::NonNegativeDecimal{core::Decimal{staffSize}});
        if (staffScaling >= 0.0)
        {
            size.setScaling(core::NonNegativeDecimal{core::Decimal{staffScaling}});
        }
        staffDetails.setStaffSize(size);
    }

    myAttributes.addStaffDetails(staffDetails);
    myHasContent = true;
}

void PropertiesWriter::writeClef(int staffIndex, const api::ClefData &inClefData)
{
    core::Clef mxClef{};

    // staffIndex < 0 means a single-staff part (the caller's clefStaffIndex() collapses it), so the
    // auto rule omits the implied 1; staffIndex >= 0 is a multi-staff part, so the auto rule emits
    // staffIndex + 1. writeStaffNumber forces the decision either way (round-trip fidelity).
    bool includeNumber = staffIndex >= 0;
    if (inClefData.writeStaffNumber == api::Bool::yes)
    {
        includeNumber = true;
    }
    else if (inClefData.writeStaffNumber == api::Bool::no)
    {
        includeNumber = false;
    }
    if (includeNumber)
    {
        mxClef.setNumber(core::StaffNumber{staffIndex >= 0 ? staffIndex + 1 : 1});
    }

    Converter converter;
    core::ClefGroup cg{};
    cg.setSign(converter.convert(inClefData.symbol));

    if (inClefData.isLineSpecified)
    {
        cg.setLine(core::StaffLinePosition{inClefData.line});
    }

    if (inClefData.isOctaveChangeSpecified)
    {
        cg.setClefOctaveChange(inClefData.octaveChange);
    }

    if (inClefData.printObject != api::Bool::unspecified)
    {
        mxClef.setPrintObject(converter.convert(inClefData.printObject));
    }

    mxClef.setClef(cg);
    myAttributes.addClef(mxClef);
    myHasContent = true;
}

void PropertiesWriter::writePartSymbol(const api::PartSymbolData &inPartSymbolData)
{
    Converter converter;
    core::PartSymbol ps;
    ps.setValue(converter.convert(inPartSymbolData.value));
    if (inPartSymbolData.topStaff >= 1)
    {
        ps.setTopStaff(core::StaffNumber{inPartSymbolData.topStaff});
    }
    if (inPartSymbolData.bottomStaff >= 1)
    {
        ps.setBottomStaff(core::StaffNumber{inPartSymbolData.bottomStaff});
    }
    myAttributes.setPartSymbol(ps);
    myHasContent = true;
}

void PropertiesWriter::writeTranspose(int staffIndex, const api::TransposeData &inTransposeData)
{
    auto xpose = Converter::convertToTranspose(inTransposeData);

    if (staffIndex >= 0)
    {
        xpose.setNumber(core::StaffNumber{staffIndex + 1});
    }

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
