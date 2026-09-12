// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

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
using DocumentPtr = std::shared_ptr<Document>;
} // namespace core

namespace api
{
// A MusicXML document that you own. Parse one from a file or a stream, or
// author one from ScoreData, then read the score back out of it or write it
// to disk. The document is freed automatically when it goes out of scope;
// mx keeps no registry of documents and tracks no ids. A document cannot be
// copied, only moved.
class MusicXml
{
  public:
    // parses a .musicxml file. Errors: ioError (file open/read),
    // xmlSyntaxError (the bytes are not XML), or the mirrored core parse
    // errors.
    static Result<MusicXml> fromFile(const std::string &filePath);

    // parses from any character stream. Same errors as fromFile, minus the
    // file I/O.
    static Result<MusicXml> fromStream(std::istream &stream);

    MusicXml(const MusicXml &other) = delete;
    MusicXml &operator=(const MusicXml &other) = delete;
    MusicXml(MusicXml &&other) noexcept;
    MusicXml &operator=(MusicXml &&other) noexcept;
    ~MusicXml();

    // writes the document to a file. Errors: ioError on write failure.
    Result<void> writeToFile(const std::string &filePath) const;

    // writes the document to a character stream. Fails only with
    // internalError.
    Result<void> writeToStream(std::ostream &stream) const;

    // access to the underlying core document for requirements that ScoreData
    // does not meet. Prefer the score functions above; the core model is a
    // much larger interface and it is not frozen the way mx::api is.
    const core::Document &getCoreDocument() const;

  private:
    MusicXml(core::DocumentPtr &&coreDocument, bool writeMxVersion);
    class Impl;
    std::unique_ptr<Impl> myImpl;

    friend Result<ScoreData> getScore(const MusicXml &document);
    friend Result<MusicXml> fromScore(const ScoreData &score);
};

// reads the score out of the document. The document stays alive and can be
// read again or written out. Fails only with internalError.
Result<ScoreData> getScore(const MusicXml &document);

// reads the score out of the document and consumes it: the underlying tree
// is freed when this function returns rather than when your MusicXml binding
// goes out of scope. Pass the document with std::move, or hand over the
// Result's value directly. Fails only with internalError.
Result<ScoreData> intoScore(MusicXml document);

// authors a new document from ScoreData. CAN fail: when the ScoreData
// describes something the core model will not represent (e.g. more than 8
// beams), the error is returned rather than silently dropping data.
Result<MusicXml> fromScore(const ScoreData &score);
} // namespace api
} // namespace mx
