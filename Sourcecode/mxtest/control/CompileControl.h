// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#define MX_COMPILE_API_TESTS
#define MX_COMPILE_API_ROUNDTRIP
#define MX_COMPILE_CORE_TESTS
#define MX_COMPILE_IMPL_TESTS
#define MX_COMPILE_IMPORT_TESTS
#define MX_COMPILE_NEW_DECIMAL_TESTS
#define MX_COMPILE_UTILTIY_TESTS
#define MX_COMPILE_XML_TESTS

// use this to restrict the size of the files that
// the test run will open (compile-time constant).
// 0 indicates no limit
constexpr const int MX_COMPILE_MAX_FILE_SIZE_BYTES = 0; // 1024 * 3; // (1024 * 1024);
