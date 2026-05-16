// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mx/core/Elements.h"
#include "mxtest/core/HelperFunctions.h"

namespace mxtest
{
mx::core::GroupingPtr tgenGrouping(TestMode v);
void tgenGroupingExpected(std::ostream &os, int indentLevel, TestMode v);
} // namespace mxtest

#endif
