// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/Result.h"
#include "mx/api/ScoreData.h"

#include <memory>
#include <ostream>

namespace mx
{
namespace core
{
class Document;
using DocumentPtr = std::shared_ptr<Document>;
} // namespace core

namespace api
{
// The mx::api error channel: no exceptions escape this boundary; failures
// speak Result/ApiError, and any exception from
// below becomes ResultCode::internalError.
class DocumentManager
{
  public:
    DocumentManager(const DocumentManager &other) = delete;
    DocumentManager(DocumentManager &&other) = delete;
    DocumentManager &operator=(const DocumentManager &other) = delete;
    DocumentManager &operator=(DocumentManager &&other) = delete;
    ~DocumentManager();

    // this class is a singleton, get it like this
    // auto& docMngr = DocumentManager::getInstance()
    static DocumentManager &getInstance();

    // creates a MusicXML document from a file and returns the document's
    // ID number. Errors: ioError (file open/read), xmlSyntaxError (the
    // bytes are not XML), or the mirrored core parse errors.
    Result<int> createFromFile(const std::string &filePath);

    // creates a MusicXML document from a character stream and returns the
    // document's ID number. Same errors as createFromFile, minus file I/O.
    Result<int> createFromStream(std::istream &stream);

    // creates a MusicXML document from a Score structure and returns the
    // document's ID number. CAN fail: when the ScoreData describes
    // something the core model will not represent (e.g. more than 8
    // beams), the error is returned rather than silently dropping data.
    Result<int> createFromScore(const ScoreData &score);

    // saves an existing MusicXML document to a file.
    // Errors: badDocumentId, ioError on write failure.
    Result<void> writeToFile(int documentId, const std::string &filePath) const;

    // saves an existing MusicXML document to a character stream.
    // Errors: badDocumentId.
    Result<void> writeToStream(int documentId, std::ostream &stream) const;

    // retrieves the data from an existing document and returns it in the
    // Score structure. Errors: badDocumentId.
    Result<ScoreData> getData(int documentId) const;

    // destroys the shared_ptr which is holding the document internally and
    // stops tracking ownership of the document. idempotent: destroying a
    // nonexistent id is a silent no-op (that is a feature).
    void destroyDocument(int documentId);

    // avoid using this function if possible, only use this function
    // if your requirements are not met by the Score structure
    // note: returns a nullptr if the documentId is bad.  if you
    // hold a DocumentPtr and call destroyDocument then the
    // DocumentManager will no longer know about the object, but the
    // object will still exist
    mx::core::DocumentPtr getDocument(int documentId) const;

    // returns a unique number and increments the unique number
    // generator. this is here as an aid to any client code that
    // needs unique numbers since DocumentManager is already a
    // thread-locking singleton, it can easily implement this.
    int getUniqueId();

  private:
    DocumentManager();
    class Impl;
    std::unique_ptr<Impl> myImpl;
};
} // namespace api
} // namespace mx
