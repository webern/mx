// Copyright (c) Matthew James Briggs

#pragma once
#include "private/Throw.h"

#define EZXML_THROW_XNULL EZXML_THROW( "The internal object is null. This probably means that XDoc has gone out of scope causing all of its children to be destroyed." );
