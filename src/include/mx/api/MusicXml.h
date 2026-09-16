// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Diagnostics.h"
#include "mx/api/Result.h"
#include "mx/api/ScoreData.h"

#include <iosfwd>
#include <memory>
#include <string>

namespace mx
{
namespace core
{
class Document;
} // namespace core

namespace api
{
// A MusicXML document, either parsed or constructed from ScoreData.
class MusicXml
{
  public:
    // Parses a MusicXML file. Logical errors and caught exceptions are
    // represented by an error result.
    static Result<MusicXml> fromFile(const std::string &filePath);

    // Parses a MusicXML document from a character stream. Logical errors and
    // caught exceptions are represented by an error result.
    static Result<MusicXml> fromStream(std::istream &stream);

    MusicXml(const MusicXml &other) = delete;
    MusicXml &operator=(const MusicXml &other) = delete;
    MusicXml(MusicXml &&other);
    MusicXml &operator=(MusicXml &&other) noexcept;
    ~MusicXml();

    // A deep copy of the document.
    MusicXml clone() const;

    // Writes the document to a file. Logical errors and caught exceptions
    // are represented by an error result.
    Result<void> writeToFile(const std::string &filePath) const;

    // Writes the document to a character stream.
    Result<void> writeToStream(std::ostream &stream) const;

    // This is an escape hatch in case mx::api does not do what you need and
    // you want to edit the core DOM directly. You will need to include the
    // private mx::core headers in your header search paths to do so. Not
    // recommended, try opening an issue first!
    //
    // The reference is only good for as long as this MusicXml is alive and
    // you have not moved it away: do not keep it past a std::move of this
    // object into another MusicXml or into intoScore, which destroys the
    // document when it returns.
    core::Document &getCoreDocument();
    const core::Document &getCoreDocument() const;

  private:
    MusicXml();
    MusicXml(core::Document document, bool writeMxVersion);
    class Impl;
    std::unique_ptr<Impl> myImpl;

    friend Result<ScoreData> getScore(const MusicXml &document, Diagnostics &diagnostics);
    friend Result<MusicXml> fromScore(const ScoreData &score, Diagnostics &diagnostics);
};

// Reads the score out of the document. The document stays alive and can be
// read again or written out. Pass Diagnostics to observe adjustments made
// while translating the document.
Result<ScoreData> getScore(const MusicXml &document);
Result<ScoreData> getScore(const MusicXml &document, Diagnostics &diagnostics);

// Reads the score out of the document and consumes it: the underlying tree
// is freed when this function returns rather than when your MusicXml binding
// goes out of scope. Pass the document with std::move, or hand over the
// Result's value directly.
Result<ScoreData> intoScore(MusicXml document);
Result<ScoreData> intoScore(MusicXml document, Diagnostics &diagnostics);

// Authors a new document from ScoreData. Fails with an error result when the
// ScoreData describes something the core model will not represent (e.g. more
// than 8 beams) rather than silently dropping data. Pass Diagnostics to
// observe recoverable authoring decisions.
Result<MusicXml> fromScore(const ScoreData &score);
Result<MusicXml> fromScore(const ScoreData &score, Diagnostics &diagnostics);

} // namespace api
} // namespace mx
