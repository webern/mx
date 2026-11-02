// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mxpugi/mxpugi.hpp"

#include <string>
#include <vector>

namespace mxtest
{
namespace corert
{

// One substitution rule deserialized from a `<replace>` child in a
// `.fixup.xml` sidecar. A rule matches when both `name` and `value` are
// exact-equal to the node being visited.
struct Fixup
{
    enum class Type
    {
        Element,
        Attribute,
    };

    Type type = Type::Element;
    std::string name;
    std::string value;
    std::string replacementValue;
};

// Loads a `.fixup.xml` sidecar (if any) for a given input file and applies
// its substitution rules to the *expected* document before the round-trip
// comparison (see data/README.md). mx clamps out-of-bounds values on
// import; the sidecar patches the expected tree to match the clamped
// output. Operations other than `<replace>` are skipped, not errors, so a
// sidecar written for a richer harness leaves this one green.
class Fixer
{
  public:
    explicit Fixer(const std::string &inputFilePath);

    bool hasFixups() const
    {
        return !myFixups.empty();
    }

    void applyToExpected(mxpugi::xml_document &expected) const;

  private:
    std::vector<Fixup> myFixups;
};

} // namespace corert
} // namespace mxtest
