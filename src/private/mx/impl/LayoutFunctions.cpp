// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/LayoutFunctions.h"
#include "mx/api/ScoreData.h"
#include "mx/core/Decimal.h"
#include "mx/core/NameToken.h"
#include "mx/core/generated/AllMarginsGroup.h"
#include "mx/core/generated/Appearance.h"
#include "mx/core/generated/Defaults.h"
#include "mx/core/generated/Distance.h"
#include "mx/core/generated/DistanceType.h"
#include "mx/core/generated/EmptyFont.h"
#include "mx/core/generated/LayoutGroup.h"
#include "mx/core/generated/LeftRightMarginsGroup.h"
#include "mx/core/generated/LineWidth.h"
#include "mx/core/generated/LineWidthType.h"
#include "mx/core/generated/LyricFont.h"
#include "mx/core/generated/Millimeters.h"
#include "mx/core/generated/NoteSize.h"
#include "mx/core/generated/NoteSizeType.h"
#include "mx/core/generated/OtherAppearance.h"
#include "mx/core/generated/PageLayout.h"
#include "mx/core/generated/PageLayoutGroup.h"
#include "mx/core/generated/PageMargins.h"
#include "mx/core/generated/Scaling.h"
#include "mx/core/generated/StaffLayout.h"
#include "mx/core/generated/SystemLayout.h"
#include "mx/core/generated/SystemMargins.h"
#include "mx/core/generated/Tenths.h"
#include "mx/impl/FontFunctions.h"

namespace mx
{
namespace impl
{

static core::Tenths toTenths(double value)
{
    return core::Tenths{core::Decimal{value > 0.0 ? value : 0.0}};
}

static std::vector<core::PageMargins> createPageMargins(const api::PageMarginsData &inPageMargins)
{
    std::vector<core::PageMargins> out;
    if (!inPageMargins.isUsed())
    {
        return out;
    }

    const bool areEvenOddSame = inPageMargins.same();

    if (inPageMargins.odd)
    {
        core::PageMargins m;
        m.setType(areEvenOddSame ? core::MarginType::both() : core::MarginType::odd());
        core::AllMarginsGroup allMargins;
        core::LeftRightMarginsGroup lrm;
        lrm.setLeftMargin(core::Tenths{core::Decimal{inPageMargins.odd->left}});
        lrm.setRightMargin(core::Tenths{core::Decimal{inPageMargins.odd->right}});
        allMargins.setLeftRightMargins(lrm);
        allMargins.setTopMargin(core::Tenths{core::Decimal{inPageMargins.odd->top}});
        allMargins.setBottomMargin(core::Tenths{core::Decimal{inPageMargins.odd->bottom}});
        m.setAllMargins(allMargins);
        out.push_back(std::move(m));
    }

    if (inPageMargins.even && !areEvenOddSame)
    {
        core::PageMargins m;
        m.setType(core::MarginType::even());
        core::AllMarginsGroup allMargins;
        core::LeftRightMarginsGroup lrm;
        lrm.setLeftMargin(core::Tenths{core::Decimal{inPageMargins.even->left}});
        lrm.setRightMargin(core::Tenths{core::Decimal{inPageMargins.even->right}});
        allMargins.setLeftRightMargins(lrm);
        allMargins.setTopMargin(core::Tenths{core::Decimal{inPageMargins.even->top}});
        allMargins.setBottomMargin(core::Tenths{core::Decimal{inPageMargins.even->bottom}});
        m.setAllMargins(allMargins);
        out.push_back(std::move(m));
    }

    return out;
}

static core::PageLayout createPageLayout(const api::PageLayoutData &inPageLayout)
{
    core::PageLayout outPageLayout;

    if (inPageLayout.size)
    {
        core::PageLayoutGroup group;
        group.setPageHeight(core::Tenths{core::Decimal{inPageLayout.size->height}});
        group.setPageWidth(core::Tenths{core::Decimal{inPageLayout.size->width}});
        outPageLayout.setGroup(group);
    }

    auto marginsVec = createPageMargins(inPageLayout.margins);
    for (auto &m : marginsVec)
    {
        (void)outPageLayout.addPageMargins(std::move(m));
    }

    return outPageLayout;
}

void addDefaultsData(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup)
{
    addScaling(inDefaults, outScoreHeaderGroup);
    addPageLayout(inDefaults.pageLayout, outScoreHeaderGroup);
    addSystemMargins(inDefaults, outScoreHeaderGroup);
    addAppearance(inDefaults, outScoreHeaderGroup);
    addDefaultsFonts(inDefaults, outScoreHeaderGroup);
}

void addDefaultsFonts(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup)
{
    if (!inDefaults.musicFont.has_value() && !inDefaults.wordFont.has_value() && inDefaults.lyricFonts.empty())
    {
        return;
    }

    auto defaults = outScoreHeaderGroup.defaults().value_or(core::Defaults{});

    if (inDefaults.musicFont.has_value())
    {
        core::EmptyFont musicFont{};
        setAttributesFromFontData(*inDefaults.musicFont, musicFont);
        defaults.setMusicFont(musicFont);
    }

    if (inDefaults.wordFont.has_value())
    {
        core::EmptyFont wordFont{};
        setAttributesFromFontData(*inDefaults.wordFont, wordFont);
        defaults.setWordFont(wordFont);
    }

    if (!inDefaults.lyricFonts.empty())
    {
        std::vector<core::LyricFont> lyricFonts;
        for (const auto &lf : inDefaults.lyricFonts)
        {
            core::LyricFont coreLyricFont{};
            if (!lf.number.empty())
            {
                coreLyricFont.setNumber(core::NameToken{lf.number});
            }
            if (!lf.name.empty())
            {
                coreLyricFont.setName(lf.name);
            }
            setAttributesFromFontData(lf.font, coreLyricFont);
            lyricFonts.emplace_back(std::move(coreLyricFont));
        }
        defaults.setLyricFont(std::move(lyricFonts));
    }

    outScoreHeaderGroup.setDefaults(defaults);
}

void addScaling(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup)
{
    if (inDefaults.scalingMillimeters <= 0 && inDefaults.scalingTenths <= 0)
    {
        return;
    }

    auto defaults = outScoreHeaderGroup.defaults().value_or(core::Defaults{});
    core::Scaling scaling{};

    core::Millimeters mm;
    mm.setValue(core::Decimal{inDefaults.scalingMillimeters > 0 ? inDefaults.scalingMillimeters : 0.0});
    scaling.setMillimeters(mm);

    core::Tenths t;
    t.setValue(core::Decimal{inDefaults.scalingTenths > 0 ? inDefaults.scalingTenths : 0.0});
    scaling.setTenths(t);

    defaults.setScaling(scaling);
    outScoreHeaderGroup.setDefaults(defaults);
}

void addPageLayout(const api::PageLayoutData &inPageLayout, core::ScoreHeaderGroup &outScoreHeaderGroup)
{
    if (!inPageLayout.isUsed())
    {
        return;
    }

    auto defaults = outScoreHeaderGroup.defaults().value_or(core::Defaults{});
    auto layout = defaults.layout();
    layout.setPageLayout(createPageLayout(inPageLayout));
    defaults.setLayout(layout);
    outScoreHeaderGroup.setDefaults(defaults);
}

void addSystemMargins(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup)
{
    bool needsDefaults = false;
    auto defaults = outScoreHeaderGroup.defaults().value_or(core::Defaults{});
    auto layout = defaults.layout();
    auto systemLayout = layout.systemLayout().value_or(core::SystemLayout{});

    if (inDefaults.systemLayout.systemDistance)
    {
        systemLayout.setSystemDistance(toTenths(inDefaults.systemLayout.systemDistance.value()));
        needsDefaults = true;
    }

    if (inDefaults.systemLayout.topSystemDistance)
    {
        systemLayout.setTopSystemDistance(toTenths(inDefaults.systemLayout.topSystemDistance.value()));
        needsDefaults = true;
    }

    if (inDefaults.systemLayout.margins)
    {
        const auto &margins = inDefaults.systemLayout.margins.value();
        core::SystemMargins sm;
        core::LeftRightMarginsGroup lrm;
        lrm.setLeftMargin(toTenths(margins.left));
        lrm.setRightMargin(toTenths(margins.right));
        sm.setLeftRightMargins(lrm);
        systemLayout.setSystemMargins(sm);
        needsDefaults = true;
    }

    if (inDefaults.systemLayout.staffDistance)
    {
        core::StaffLayout staffLayout;
        staffLayout.setStaffDistance(toTenths(inDefaults.systemLayout.staffDistance.value()));
        layout.addStaffLayout(staffLayout);
        needsDefaults = true;
    }

    if (needsDefaults)
    {
        layout.setSystemLayout(systemLayout);
        defaults.setLayout(layout);
        outScoreHeaderGroup.setDefaults(defaults);
    }
}

void addAppearance(const api::DefaultsData &inDefaults, core::ScoreHeaderGroup &outScoreHeaderGroup)
{
    if (inDefaults.appearance.empty())
    {
        return;
    }

    auto defaults = outScoreHeaderGroup.defaults().value_or(core::Defaults{});
    auto appearance = defaults.appearance().value_or(core::Appearance{});

    for (const auto &appearanceData : inDefaults.appearance)
    {
        if (appearanceData.appearanceType == api::AppearanceType::LineWidth)
        {
            core::LineWidth lw;
            lw.setType(core::LineWidthType{appearanceData.appearanceSubType});
            lw.setValue(toTenths(appearanceData.value));
            appearance.addLineWidth(lw);
        }
        else if (appearanceData.appearanceType == api::AppearanceType::NoteSize)
        {
            core::NoteSize ns;
            ns.setType(core::NoteSizeType::parse(appearanceData.appearanceSubType));
            ns.setValue(core::NonNegativeDecimal{core::Decimal{appearanceData.value}});
            appearance.addNoteSize(ns);
        }
        else if (appearanceData.appearanceType == api::AppearanceType::Distance)
        {
            core::Distance di;
            di.setType(core::DistanceType{appearanceData.appearanceSubType});
            di.setValue(toTenths(appearanceData.value));
            appearance.addDistance(di);
        }
        else if (appearanceData.appearanceType == api::AppearanceType::OtherAppearance)
        {
            core::OtherAppearance oa;
            oa.setType(appearanceData.appearanceSubType);
            oa.setValue(std::to_string(appearanceData.value));
            appearance.addOtherAppearance(oa);
        }
    }

    defaults.setAppearance(appearance);
    outScoreHeaderGroup.setDefaults(defaults);
}

api::DefaultsData createDefaults(const core::ScoreHeaderGroup &inScoreHeaderGroup)
{
    api::DefaultsData defaults;
    addScaling(inScoreHeaderGroup, defaults);
    addPageMargins(inScoreHeaderGroup, defaults);
    addSystemMargins(inScoreHeaderGroup, defaults);
    addStaffLayout(inScoreHeaderGroup, defaults);
    addAppearance(inScoreHeaderGroup, defaults);
    addDefaultsFonts(inScoreHeaderGroup, defaults);
    return defaults;
}

void addDefaultsFonts(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults)
{
    outDefaults.musicFont = std::nullopt;
    outDefaults.wordFont = std::nullopt;
    outDefaults.lyricFonts.clear();

    if (!inScoreHeaderGroup.defaults().has_value())
    {
        return;
    }

    const auto &defaults = *inScoreHeaderGroup.defaults();

    if (defaults.musicFont().has_value())
    {
        outDefaults.musicFont = getFontData(*defaults.musicFont());
    }

    if (defaults.wordFont().has_value())
    {
        outDefaults.wordFont = getFontData(*defaults.wordFont());
    }

    for (const auto &lf : defaults.lyricFont())
    {
        api::LyricFontData data{};
        if (lf.number().has_value())
        {
            data.number = lf.number()->value();
        }
        if (lf.name().has_value())
        {
            data.name = *lf.name();
        }
        data.font = getFontData(lf);
        outDefaults.lyricFonts.emplace_back(std::move(data));
    }
}

void addScaling(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults)
{
    if (inScoreHeaderGroup.defaults().has_value() && inScoreHeaderGroup.defaults()->scaling().has_value())
    {
        const auto &scaling = *inScoreHeaderGroup.defaults()->scaling();
        outDefaults.scalingMillimeters = scaling.millimeters().value().value();
        outDefaults.scalingTenths = scaling.tenths().value().value();
    }
    else
    {
        outDefaults.scalingMillimeters = -1.0;
        outDefaults.scalingTenths = -1.0;
    }
}

void addPageMargins(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults)
{
    if (!inScoreHeaderGroup.defaults().has_value() || !inScoreHeaderGroup.defaults()->layout().pageLayout().has_value())
    {
        return;
    }

    const auto &pageLayout = *inScoreHeaderGroup.defaults()->layout().pageLayout();

    if (pageLayout.group().has_value())
    {
        outDefaults.pageLayout.size = api::SizeData{pageLayout.group()->pageHeight().value().value(),
                                                    pageLayout.group()->pageWidth().value().value()};
    }

    for (const auto &m : pageLayout.pageMargins())
    {
        const auto &allMargins = m.allMargins();
        const auto &lrm = allMargins.leftRightMargins();
        api::MarginsData md{lrm.leftMargin().value().value(), lrm.rightMargin().value().value(),
                            allMargins.topMargin().value().value(), allMargins.bottomMargin().value().value()};

        const bool hasBoth = !m.type().has_value() || *m.type() == core::MarginType::both();

        if (hasBoth || *m.type() == core::MarginType::odd())
        {
            outDefaults.pageLayout.margins.odd = md;
        }

        if (hasBoth || *m.type() == core::MarginType::even())
        {
            outDefaults.pageLayout.margins.even = md;
        }
    }
}

void addSystemMargins(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults)
{
    if (!inScoreHeaderGroup.defaults().has_value() ||
        !inScoreHeaderGroup.defaults()->layout().systemLayout().has_value())
    {
        return;
    }

    const auto &systemLayout = *inScoreHeaderGroup.defaults()->layout().systemLayout();

    if (systemLayout.topSystemDistance().has_value())
    {
        outDefaults.systemLayout.topSystemDistance = systemLayout.topSystemDistance()->value().value();
    }

    if (systemLayout.systemDistance().has_value())
    {
        outDefaults.systemLayout.systemDistance = systemLayout.systemDistance()->value().value();
    }

    if (systemLayout.systemMargins().has_value())
    {
        const auto &lrm = systemLayout.systemMargins()->leftRightMargins();
        outDefaults.systemLayout.margins =
            api::LeftRight{lrm.leftMargin().value().value(), lrm.rightMargin().value().value()};
    }
}

void addStaffLayout(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults)
{
    if (!inScoreHeaderGroup.defaults().has_value())
    {
        return;
    }

    const auto &staffLayouts = inScoreHeaderGroup.defaults()->layout().staffLayout();
    if (staffLayouts.empty())
    {
        return;
    }

    if (staffLayouts.front().staffDistance().has_value())
    {
        outDefaults.systemLayout.staffDistance = staffLayouts.front().staffDistance()->value().value();
    }
}

void addAppearance(const core::ScoreHeaderGroup &inScoreHeaderGroup, api::DefaultsData &outDefaults)
{
    outDefaults.appearance.clear();

    if (!inScoreHeaderGroup.defaults().has_value() || !inScoreHeaderGroup.defaults()->appearance().has_value())
    {
        return;
    }

    const auto &appearance = *inScoreHeaderGroup.defaults()->appearance();

    for (const auto &lw : appearance.lineWidth())
    {
        api::AppearanceData data{};
        data.appearanceType = api::AppearanceType::LineWidth;
        data.appearanceSubType = std::string{lw.type().toString()};
        data.value = lw.value().value().value();
        outDefaults.appearance.emplace_back(std::move(data));
    }

    for (const auto &ns : appearance.noteSize())
    {
        api::AppearanceData data{};
        data.appearanceType = api::AppearanceType::NoteSize;
        data.appearanceSubType = std::string{ns.type().toString()};
        data.value = ns.value().value().value();
        outDefaults.appearance.emplace_back(std::move(data));
    }

    for (const auto &di : appearance.distance())
    {
        api::AppearanceData data{};
        data.appearanceType = api::AppearanceType::Distance;
        data.appearanceSubType = std::string{di.type().toString()};
        data.value = di.value().value().value();
        outDefaults.appearance.emplace_back(std::move(data));
    }

    for (const auto &oa : appearance.otherAppearance())
    {
        api::AppearanceData data{};
        data.appearanceType = api::AppearanceType::OtherAppearance;
        data.appearanceSubType = oa.type();
        data.value = 0.0;
        outDefaults.appearance.emplace_back(std::move(data));
    }
}
} // namespace impl
} // namespace mx
