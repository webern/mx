#pragma once
#include "mx/utility/Throw.h"

#define MX_THROW_XNULL MX_THROW( "The internal object is null. This probably means that XDoc has gone out of scope causing all of its children to be destroyed." );