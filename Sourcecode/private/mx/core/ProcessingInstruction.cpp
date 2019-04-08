// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/ProcessingInstruction.h"
#include "mx/utility/Throw.h"

#include <sstream>

namespace mx
{
    namespace core
    {

////////////////////////////////////////////////////////////////////////////////
// CTOR AND COPY ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

        ProcessingInstruction::ProcessingInstruction()
        : myName{ "default-name" }
        , myData{}
        , mIsChild{true}
        {

        }


        ProcessingInstruction::ProcessingInstruction( StringType inName, StringType inData )
        : ProcessingInstruction{}
        {
            this->setName( std::move( inName ) );
            this->setData( std::move( inData ) );
        }

////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

        const StringType& ProcessingInstruction::getName() const
        {
            return myName;
        }


        void ProcessingInstruction::setName( StringType inName )
        {
            if( !inName.empty() )
            {
                myName = std::move( inName );
            }
        }


        const StringType& ProcessingInstruction::getData() const
        {
            return myData;
        }


        void ProcessingInstruction::setData( StringType inData )
        {
            myData = std::move( inData );
        }


        std::ostream& ProcessingInstruction::toStream( std::ostream& os ) const
        {
            os << "<?";
            os << getName();
            os << " ";
            os << getData();
            os << "?>";
            return os;
        }


        StringType ProcessingInstruction::toString() const
        {
            std::stringstream ss;
            this->toStream( ss );
            return ss.str();
        }


        bool ProcessingInstruction::getIsChild() const
        {
            return mIsChild;
        }


        void ProcessingInstruction::setIsChild( bool inValue )
        {
            mIsChild = inValue;
        }

////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    }
}
