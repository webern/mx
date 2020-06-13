// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/AppearanceData.h"
#include "mx/api/PageLayoutData.h"
#include "mx/api/SystemLayoutData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        /// Corresponds to the `<defaults>` element which is part of the
        /// `ScoreHeaderGroup`. Sets the global defaults for the score such as
        /// page size, system spacing, and the all-important `<tenths>` and
        /// `<millimeters>` values.
        class DefaultsData
        {
        public:

            /// Sets the standard staff size for the score, in millimeters.
            /// However, it does not work the way you might expect. The
            /// millimeters value cannot be specified independently of the
            /// tenths value (below). The tenths value says, basically, how
            /// many tenths of a staff space we are specifying for the scaling
            /// for. The simplest way to understand this is to set tenths to 40,
            /// by which you are saying the scale you are providing is for 40
            /// tenths of a staff space, which is the same as 4 staff spaces,
            /// with is the same as a normal staff. So if you set tenths to 40,
            /// then millimeters will specify the size of your staff.
            ///
            /// Here's an example, if I create a default document with Finale
            /// and export it to MusicXML, it writes this:
            ///
            ///
            ///  <defaults>
            ///    <scaling>
            ///      <millimeters>7.2319</millimeters>
            ///      <tenths>40</tenths>
            ///      ...
            ///    </scaling>
            ///  </defaults>
            ///
            /// In this example, Finale is saying that the size of the staff is
            /// 7.2319 millimeters.
            /// 
            /// Please note: we plan to provide types for specifying tenths and
            /// millimeters in the future and this field will experience a
            /// breaking change: https://github.com/webern/mx/issues/99
            ///
            /// Here is the documentation from MusicXML:
            /// 
            /// The millimeters type is a number representing millimeters. This
            /// is used in the scaling element to provide a default scaling from
            /// tenths to physical units.
            Double scalingMillimeters;

            /// All measurements in the score are in 'tenths', which is one
            /// tenth of a staff space. In other words, the size of a staff
            /// space is 10 `tenths`.
            /// 
            /// Please note: we plan to provide types for specifying tenths and
            /// millimeters in the future and this field will experience a
            /// breaking change: https://github.com/webern/mx/issues/99
            ///
            /// Here is the documentation from MusicXML:
            ///
            /// The tenths type is a number representing tenths of interline 
            /// staff space (positive or negative). Both integer and decimal
            /// values are allowed, such as 5 for a half space and 2.5 for a
            /// quarter space. Interline space is measured from the middle of a
            /// staff line.
            ///
            /// Distances in a MusicXML file are measured in tenths of staff
            /// space. Tenths are then scaled to millimeters within the scaling
            /// element, used in the defaults element at the start of a score.
            /// Individual staves can apply a scaling factor to adjust staff
            /// size. When a MusicXML element or attribute refers to tenths, it
            /// means the global tenths defined by the scaling element, not the
            /// local tenths as adjusted by the staff-size element.
            Double scalingTenths;

            inline Double tenthsPerMillimeter() const
            {
                return scalingTenths / scalingMillimeters;
            }

            inline Double tenthsPerInch() const
            {
                return tenthsPerMillimeter() * 25.4;
            }

            /// Page layout. Note all members of PageLayoutData are optional.
            PageLayoutData pageLayout;

            /// System layout. Note all members of SystemLayoutData are optional.
            SystemLayoutData systemLayout;

            std::vector<AppearanceData> appearance;
            
            /// Measure numbering setting, at the global level, will be stated
            /// in first measure's <print> tag. This can can be overridden by a
            /// value in the Measure.
            MeasureNumbering measureNumbering; 
            
            DefaultsData()
            : scalingMillimeters{ -1.0 }
            , scalingTenths{ -1.0 }
            , pageLayout{}
            , systemLayout{}
            , appearance{}
            , measureNumbering{ MeasureNumbering::unspecified }
            {

            }
        };
        
        MXAPI_EQUALS_BEGIN( DefaultsData )
        MXAPI_EQUALS_MEMBER( pageLayout )
        MXAPI_EQUALS_MEMBER( systemLayout )
        MXAPI_EQUALS_MEMBER( appearance )
        MXAPI_EQUALS_MEMBER( measureNumbering )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( DefaultsData );
    }
}
