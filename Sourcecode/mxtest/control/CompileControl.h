// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#define MX_COMPILE_API_TESTS
#define MX_COMPILE_API_ROUNDTRIP
//#define MX_COMPILE_CORE_TESTS
#define MX_COMPILE_IMPL_TESTS
#define MX_COMPILE_IMPORT_TESTS
#define MX_COMPILE_UTILTIY_TESTS
#define MX_COMPILE_XML_TESTS

// use this to restrict the size of the files that
// the test run will open (compile-time constant).
// 0 indicates no limit
constexpr const int MX_COMPILE_MAX_FILE_SIZE_BYTES = (5 * 1024);
