// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mx/core/Elements.h"
#include "mxtest/core/HelperFunctions.h"

namespace mxtest
{
mx::core::BarlinePtr tgenBarline(TestMode v);
void tgenBarlineExpected(std::ostream &os, int indentLevel, TestMode v);
} // namespace mxtest

#endif
