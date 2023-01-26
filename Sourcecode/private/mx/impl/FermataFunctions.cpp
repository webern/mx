// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Fermata.h"
#include "mx/impl/FermataFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
    namespace impl
    {

    FermataFunctions::FermataFunctions( const core::Fermata& inFermata, impl::Cursor inCursor )
        : myFermata{ inFermata }
        , myCursor{ inCursor }
        {
            
        }
        
        api::MarkData FermataFunctions::parseFermata() const
        {
            const auto& attr = myFermata.getAttributes();
            const auto shape = myFermata.getValue();
            auto markType = api::MarkType::fermata;
            
            if( shape == core::FermataShape::emptystring )
            {
                if( !attr->hasType )
                {
                    markType = api::MarkType::fermata;
                }
                else if ( attr->type == core::UprightInverted::upright )
                {
                    markType = api::MarkType::fermataUpright;
                }
                else if ( attr->type == core::UprightInverted::inverted )
                {
                    markType = api::MarkType::fermataInverted;
                }
            }
            else if( shape == core::FermataShape::normal )
            {
                if( !attr->hasType )
                {
                    markType = api::MarkType::fermataNormal;
                }
                else if ( attr->type == core::UprightInverted::upright )
                {
                    markType = api::MarkType::fermataNormalUpright;
                }
                else if ( attr->type == core::UprightInverted::inverted )
                {
                    markType = api::MarkType::fermataNormalInverted;
                }
            }
            else if( shape == core::FermataShape::angled )
            {
                if( !attr->hasType )
                {
                    markType = api::MarkType::fermataAngled;
                }
                else if ( attr->type == core::UprightInverted::upright )
                {
                    markType = api::MarkType::fermataAngledUpright;
                }
                else if ( attr->type == core::UprightInverted::inverted )
                {
                    markType = api::MarkType::fermataAngledInverted;
                }
            }
            else if( shape == core::FermataShape::square )
            {
                if( !attr->hasType )
                {
                    markType = api::MarkType::fermataSquare;
                }
                else if ( attr->type == core::UprightInverted::upright )
                {
                    markType = api::MarkType::fermataSquareUpright;
                }
                else if ( attr->type == core::UprightInverted::inverted )
                {
                    markType = api::MarkType::fermataSquareInverted;
                }
            }
            
            // Unfortunately, without doing a lot of guess word, we can't
            // know whether the correct glyph is "above" or "below"
            api::MarkData markData{ markType };
            impl::parseMarkDataAttributes( attr, markData );
            markData.tickTimePosition = myCursor.tickTimePosition;
            impl::parseMarkDataAttributes( attr, markData );
            
            return markData;
        }
    }
}
