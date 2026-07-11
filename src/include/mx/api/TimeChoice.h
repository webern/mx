// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ComplexTimeSignature.h"
#include "mx/api/TimeSignatureData.h"

#include <variant>

namespace mx
{
namespace api
{

// A variant class that switches between a simple (e.g. 3/4 or C) and a complex (e.g. 5/8+3/8) time
// signature.
//
// Justification: The simple time signature use case is much more common than the complex one, and
// MusicXML's time signature model is quite difficult to understand and use properly. In order to
// simplify the process for the most common use cases, this class offers access to a simple
// TimeSignatureData struct for simple mode and a ComplexTimeSignature class for complex mode.
//
// The <time> attributes that apply regardless of shape live here. isImplicit is an `mx::api`
// invention because, unlike MusicXML, we store a time signature at the beginning of each measure.
// If it is simply being carried over from the previous measure, then isImplicit is true.
//
// The display field correlates to <time print-object="yes/no"> and can be absent with unspecified.
//
// If a ComplexTimeSignature is used to construct something that can be represented as a simple time
// signature, then the TimeChoice class will collapse it to a simple time signature.
//
// Example (the common case):
//   measure.timeSignature = TimeChoice(TimeSignatureData{"3", "4"});
//   if (measure.timeSignature.isSimple())
//   {
//       const TimeSignatureData data = measure.timeSignature.simple();
//       assert(data.fraction.beats == "3" && data.fraction.beatType == "4");
//   }
//
// Defaults to 4/4.
class TimeChoice
{
  public:
    enum class Kind
    {
        simple,
        complex
    };

    // True when the source did not restate the time signature here (it was carried forward). The
    // writer emits a <time> only for entries where this is false. Denormalized onto every measure.
    bool isImplicit{true};

    // Print/hide the time signature (<time print-object=...>). Ignored when isImplicit.
    Bool display{Bool::unspecified};

    // Defaults to a simple, implicit 4/4.
    TimeChoice();

    TimeChoice(TimeSignatureData value);

    // Builds a complex time signature, unless the value is simple-equivalent, in which case the
    // result is a *simple* TimeChoice (auto-collapse). isImplicit/display are left at their defaults;
    // set them after construction.
    TimeChoice(ComplexTimeSignature value);

    Kind kind() const;
    bool isSimple() const;
    bool isComplex() const;

    // Returns a copy of the internally held TimeSignatureData.
    //
    // Precondition: isSimple(). If !isSimple(), a default constructed TimeSignatureData is
    // returned.
    const TimeSignatureData simple() const;

    // Returns a copy of the internally held ComplexTimeSignature.
    //
    // Precondition: isComplex(). If !isComplex(), a default constructed ComplexTimeSignature is
    // returned.
    const ComplexTimeSignature complex() const;

    bool operator==(const TimeChoice &other) const;

  private:
    std::variant<TimeSignatureData, ComplexTimeSignature> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(TimeChoice);

} // namespace api
} // namespace mx
