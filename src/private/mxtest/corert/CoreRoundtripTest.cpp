// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/corert/CoreRoundtripImpl.h"
#include "mxtest/file/PathRoot.h"

#include "cpul/cpulTestHarness.h"

#include <memory>
#include <string>
#include <vector>

namespace
{

// Skip accounting: the 4.0 corpus has nothing newer than 4.0, so any skip
// is count drift.
int g_skippedCount = 0;

// Run the core roundtrip flow for one input and report through Catch2.
//
// `relPath` is the path relative to `data/`, also the test name. On failure,
// debug files are written to `data/testOutput/corert/` and `FAIL` is invoked
// with a path-qualified message.
void runOne(const std::string &relPath)
{
    const std::string absPath = std::string(MX_REPO_ROOT_PATH) + "/data/" + relPath;
    const auto result = mxtest::corert::runCoreRoundtrip(absPath);
    if (result.skipped)
    {
        ++g_skippedCount;
        WARN("skipped " + relPath + ": " + result.message);
        return;
    }
    if (!result.ok)
    {
        if (!result.expectedXml.empty() || !result.actualXml.empty())
        {
            mxtest::corert::writeFailureFiles(relPath, result.expectedXml, result.actualXml);
        }
        FAIL("core roundtrip failed for " + relPath + ": " + result.message);
    }
}

// Dynamic Catch2 registration (design §3 Catch2 Dynamic Registration).
//
// `Catch::StringRef` does not own its storage; `TestCaseInfo` stores the test
// name as a `StringRef` (see `TestCaseInfo::TestCaseInfo` in catch.cpp).
// The backing `std::string`s must therefore outlive Catch2's runner. We hold
// them in function-local statics whose lifetime ends with the program.
//
// `Catch::AutoReg` is non-copyable and non-movable (it inherits
// `Catch::Detail::NonCopyable`), so it cannot live in a `std::vector` by
// value. We heap-allocate each one and own them through a vector of
// `std::unique_ptr`.

// Per-file invoker: holds the relative-path test name and dispatches to
// `runOne` from `invoke()`. The string is owned by the invoker (not aliased
// to the static name table) so the invoker is self-contained.
class CoreRoundtripInvoker : public Catch::ITestInvoker
{
  public:
    explicit CoreRoundtripInvoker(std::string relPath) : m_relPath(std::move(relPath))
    {
    }

    void invoke() const override
    {
        runOne(m_relPath);
    }

  private:
    std::string m_relPath;
};

// Names backing storage: one entry per discovered file, in stable order.
// `Catch::StringRef` references into these strings, so they must outlive
// Catch2's test registry — they are program-lifetime statics.
std::vector<std::string> &nameStorage()
{
    static std::vector<std::string> storage;
    return storage;
}

// AutoReg owners. Constructing an AutoReg registers the case with Catch2's
// global registry; destroying it before `main()` returns is fine, but we
// keep them alive for program lifetime to mirror the lifetime of test-case
// names they reference.
std::vector<std::unique_ptr<Catch::AutoReg>> &autoRegStorage()
{
    static std::vector<std::unique_ptr<Catch::AutoReg>> storage;
    return storage;
}

// Tag string for every registered case. Static so the StringRef stays valid.
constexpr const char *const kCoreRoundtripTag = "[core-roundtrip]";

// Static initializer: discover files and register one Catch2 case per file.
// Runs before `main()`, before Catch2's runner starts. File discovery is the
// only filesystem touch at this point — the per-case body runs later when
// Catch2 invokes each case.
struct CoreRoundtripRegistrar
{
    CoreRoundtripRegistrar()
    {
        const auto absPaths = mxtest::corert::discoverInputFiles();
        auto &names = nameStorage();
        auto &regs = autoRegStorage();
        names.reserve(absPaths.size());
        regs.reserve(absPaths.size());
        for (const auto &abs : absPaths)
        {
            names.push_back(mxtest::corert::toTestName(abs));
            const std::string &nameRef = names.back();
            auto invoker = Catch::Detail::make_unique<CoreRoundtripInvoker>(nameRef);
            const Catch::NameAndTags nameAndTags{Catch::StringRef(nameRef.c_str(), nameRef.size()),
                                                 Catch::StringRef(kCoreRoundtripTag)};
            regs.push_back(std::unique_ptr<Catch::AutoReg>(new Catch::AutoReg(
                std::move(invoker), Catch::SourceLineInfo(__FILE__, static_cast<std::size_t>(__LINE__)),
                Catch::StringRef(), nameAndTags)));
        }
    }
};

const CoreRoundtripRegistrar g_coreRoundtripRegistrar;

} // namespace

// Pinned counts: 829 eligible files, none skipped. Count drift is a failure
// even with zero individual fails, so a corpus or version-gate change is a
// conscious decision, not silent decay. Registered last (registration is
// discovery order; "zz" keeps it last alphabetically for shuffled runs too).
TEST_CASE("zz-corert-pinned-counts", "[core-roundtrip]")
{
    CHECK(mxtest::corert::discoverInputFiles().size() == 829);
    CHECK(g_skippedCount == 0);
}
