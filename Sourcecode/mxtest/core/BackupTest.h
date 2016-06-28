#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::BackupPtr tgenBackup( variant v );
    void tgenBackupExpected( std::ostream& os, int indentLevel, variant v );
}

#endif