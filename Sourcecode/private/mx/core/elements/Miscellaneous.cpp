// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Miscellaneous.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MiscellaneousField.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Miscellaneous::Miscellaneous()
        :myMiscellaneousFieldSet()
        {}


        bool Miscellaneous::hasAttributes() const
        {
            return false;
        }


        std::ostream& Miscellaneous::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Miscellaneous::streamName( std::ostream& os ) const
        {
            os << "miscellaneous";
            return os;
        }


        bool Miscellaneous::hasContents() const
        {
            return myMiscellaneousFieldSet.size() > 0;
        }


        std::ostream& Miscellaneous::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for( auto x : myMiscellaneousFieldSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( hasContents() )
            {
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        const MiscellaneousFieldSet& Miscellaneous::getMiscellaneousFieldSet() const
        {
            return myMiscellaneousFieldSet;
        }


        void Miscellaneous::removeMiscellaneousField( const MiscellaneousFieldSetIterConst& value )
        {
            if ( value != myMiscellaneousFieldSet.cend() )
            {
                myMiscellaneousFieldSet.erase( value );
            }
        }


        void Miscellaneous::addMiscellaneousField( const MiscellaneousFieldPtr& value )
        {
            if ( value )
            {
                myMiscellaneousFieldSet.push_back( value );
            }
        }


        void Miscellaneous::clearMiscellaneousFieldSet()
        {
            myMiscellaneousFieldSet.clear();
        }


        MiscellaneousFieldPtr Miscellaneous::getMiscellaneousField( const MiscellaneousFieldSetIterConst& setIterator ) const
        {
            if( setIterator != myMiscellaneousFieldSet.cend() )
            {
                return *setIterator;
            }
            return MiscellaneousFieldPtr();
        }


        bool Miscellaneous::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "miscellaneous-field", myMiscellaneousFieldSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
