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

// A measure's (or a staff's) time signature. It switches between the simple case -- the ordinary
// N/D you want 99% of the time -- and the complex case, which quarantines everything unusual
// (composite meters, senza-misura, interchangeable, unusual display symbols). Read the kind, then
// reach for asSimple() or asComplex().
//
// The whole-<time> attributes that apply regardless of shape live here, not on either leaf:
// isImplicit (was the signature restated in this measure or carried forward) and display
// (print-object show/hide).
//
// Invariant: a simple-equivalent signature is always held as simple. TimeChoice::complex collapses a
// complex value whose meter is really just a plain fraction (one fraction, no interchangeable, no
// separator, and a symbol the simple case can express) down to simple, so there is exactly one
// canonical representation and the simple case can never hide inside the complex one.
//
// Defaults to a simple, implicit 4/4.
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

    static TimeChoice simple(TimeSignatureData value);

    // Builds a complex time signature, unless the value is simple-equivalent, in which case the
    // result is a *simple* TimeChoice (auto-collapse). isImplicit/display are left at their defaults;
    // set them on the returned value.
    static TimeChoice complex(ComplexTimeSignature value);

    Kind kind() const;
    bool isSimple() const;
    bool isComplex() const;

    // Precondition: isSimple().
    const TimeSignatureData &asSimple() const;

    // Precondition: isComplex().
    const ComplexTimeSignature &asComplex() const;

    bool operator==(const TimeChoice &other) const;

  private:
    std::variant<TimeSignatureData, ComplexTimeSignature> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(TimeChoice);

} // namespace api
} // namespace mx
