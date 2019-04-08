// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/VirtualInstrument.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/VirtualLibrary.h"
#include "mx/core/elements/VirtualName.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        VirtualInstrument::VirtualInstrument()
        :myVirtualLibrary( makeVirtualLibrary() )
        ,myHasVirtualLibrary( false )
        ,myVirtualName( makeVirtualName() )
        ,myHasVirtualName( false )
        {}


        bool VirtualInstrument::hasAttributes() const
        {
            return false;
        }


        std::ostream& VirtualInstrument::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& VirtualInstrument::streamName( std::ostream& os ) const
        {
            os << "virtual-instrument";
            return os;
        }


        bool VirtualInstrument::hasContents() const
        {
            return myHasVirtualLibrary || myHasVirtualName;
        }


        std::ostream& VirtualInstrument::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasVirtualLibrary )
            {
                os << std::endl;
                myVirtualLibrary->toStream( os, indentLevel+1 );
            }
            if ( myHasVirtualName )
            {
                os << std::endl;
                myVirtualName->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        VirtualLibraryPtr VirtualInstrument::getVirtualLibrary() const
        {
            return myVirtualLibrary;
        }


        void VirtualInstrument::setVirtualLibrary( const VirtualLibraryPtr& value )
        {
            if( value )
            {
                myVirtualLibrary = value;
            }
        }


        bool VirtualInstrument::getHasVirtualLibrary() const
        {
            return myHasVirtualLibrary;
        }


        void VirtualInstrument::setHasVirtualLibrary( const bool value )
        {
            myHasVirtualLibrary = value;
        }


        VirtualNamePtr VirtualInstrument::getVirtualName() const
        {
            return myVirtualName;
        }


        void VirtualInstrument::setVirtualName( const VirtualNamePtr& value )
        {
            if( value )
            {
                myVirtualName = value;
            }
        }


        bool VirtualInstrument::getHasVirtualName() const
        {
            return myHasVirtualName;
        }


        void VirtualInstrument::setHasVirtualName( const bool value )
        {
            myHasVirtualName = value;
        }


        bool VirtualInstrument::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myVirtualLibrary, myHasVirtualLibrary ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myVirtualName, myHasVirtualName ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
