// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/xml/XDoc.h"
#include "mx/xml/XElement.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XElementIterator.h"
#include "mx/xml/XAttributeIterator.h"

#include <string>
#include <set>

// Some of the test files (particularly MuseScore and Recordare) have
// trailing zeros after decimal points.  Mx does not do this, so for
// example when we see '75.00' in an input file, we would expect Mx to
// produce '75'.  These functions will alter the 'expected' files.

namespace MxTest
{

    const std::set<std::string> decimalFields =
    {
        "top-system-distance",
        "dynamics",
        "left-margin",
        "right-margin",
        "staff-distance",
        "system-distance",
        "default-y",
        "default-x"
    };

    inline int trailingCharsToStrip( const std::string& value )
    {
        bool isDecimalFound = false;
        bool isNonZeroFound = false;
        int zeroCount = 0;
        for ( auto it = value.crbegin(); it != value.crend(); ++it )
        {
            if ( *it == '0' )
            {
                if ( !isDecimalFound && !isNonZeroFound )
                {
                    ++zeroCount;
                }
            }
            else if ( *it == '.' )
            {
                isDecimalFound = true;
                break;
            }
            else
            {
                isNonZeroFound = true;
            }
        }

        if ( !isDecimalFound )
        {
            return 0;
        }

        if ( !isNonZeroFound )
        {
            // add 1 to strip the decimal
            return zeroCount + 1;
        }

        return zeroCount;
    }


    inline void stripZeros( mx::xml::XElement& xelement )
    {
        const auto initialValue = xelement.getValue();

        if ( initialValue.empty() )
        {
            return;
        }

        const int chars = trailingCharsToStrip( initialValue );

        if ( chars == 0 )
        {
            return;
        }

        xelement.setValue( initialValue.substr( 0, initialValue.size() - chars ) );
    }


    inline void stripZeros( mx::xml::XAttribute& xattribute )
    {
        const auto initialValue = xattribute.getValue();

        if ( initialValue.empty() )
        {
            return;
        }

        const int chars = trailingCharsToStrip( initialValue );

        if ( chars == 0 )
        {
            return;
        }

        xattribute.setValue( initialValue.substr( 0, initialValue.size() - chars ) );
    }
}