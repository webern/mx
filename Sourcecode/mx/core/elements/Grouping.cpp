// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Grouping.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Feature.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Grouping::Grouping()
        :myAttributes( std::make_shared<GroupingAttributes>() )
        ,myFeatureSet()
        {}


        bool Grouping::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Grouping::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Grouping::streamName( std::ostream& os ) const
        {
            os << "grouping";
            return os;
        }


        bool Grouping::hasContents() const
        {
            return myFeatureSet.size() > 0;
        }


        std::ostream& Grouping::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myFeatureSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        GroupingAttributesPtr Grouping::getAttributes() const
        {
            return myAttributes;
        }


        void Grouping::setAttributes( const GroupingAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const FeatureSet& Grouping::getFeatureSet() const
        {
            return myFeatureSet;
        }


        void Grouping::removeFeature( const FeatureSetIterConst& value )
        {
            if ( value != myFeatureSet.cend() )
            {
                myFeatureSet.erase( value );
            }
        }


        void Grouping::addFeature( const FeaturePtr& value )
        {
            if ( value )
            {
                myFeatureSet.push_back( value );
            }
        }


        void Grouping::clearFeatureSet()
        {
            myFeatureSet.clear();
        }


        FeaturePtr Grouping::getFeature( const FeatureSetIterConst& setIterator ) const
        {
            if( setIterator != myFeatureSet.cend() )
            {
                return *setIterator;
            }
            return FeaturePtr();
        }


        bool Grouping::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "feature", myFeatureSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
