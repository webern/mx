// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Identification.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Creator.h"
#include "mx/core/elements/Encoding.h"
#include "mx/core/elements/Miscellaneous.h"
#include "mx/core/elements/Relation.h"
#include "mx/core/elements/Rights.h"
#include "mx/core/elements/Source.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Identification::Identification()
        :myCreatorSet()
        ,myRightsSet()
        ,myEncoding( makeEncoding() )
        ,myHasEncoding( false )
        ,mySource( makeSource() )
        ,myHasSource( false )
        ,myRelationSet()
        ,myMiscellaneous( makeMiscellaneous() )
        ,myHasMiscellaneous( false )
        {}


        bool Identification::hasAttributes() const
        {
            return false;
        }


        std::ostream& Identification::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Identification::streamName( std::ostream& os ) const
        {
            os << "identification";
            return os;
        }


        bool Identification::hasContents() const
        {
            return myCreatorSet.size() > 0
            || myRightsSet.size() > 0
            || myHasEncoding
            || myHasSource
            || myRelationSet.size() > 0
            || myHasMiscellaneous;
        }


        std::ostream& Identification::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myCreatorSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                for ( auto x : myRightsSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                if ( myHasEncoding )
                {
                    os << std::endl;
                    myEncoding->toStream( os, indentLevel+1 );
                }
                if ( myHasSource )
                {
                    os << std::endl;
                    mySource->toStream( os, indentLevel+1 );
                }
                for ( auto x : myRelationSet )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
                if ( myHasMiscellaneous )
                {
                    os << std::endl;
                    myMiscellaneous->toStream( os, indentLevel+1 );
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


        const CreatorSet& Identification::getCreatorSet() const
        {
            return myCreatorSet;
        }


        void Identification::removeCreator( const CreatorSetIterConst& value )
        {
            if ( value != myCreatorSet.cend() )
            {
                myCreatorSet.erase( value );
            }
        }


        void Identification::addCreator( const CreatorPtr& value )
        {
            if ( value )
            {
                myCreatorSet.push_back( value );
            }
        }


        void Identification::clearCreatorSet()
        {
            myCreatorSet.clear();
        }


        CreatorPtr Identification::getCreator( const CreatorSetIterConst& setIterator ) const
        {
            if( setIterator != myCreatorSet.cend() )
            {
                return *setIterator;
            }
            return CreatorPtr();
        }


        const RightsSet& Identification::getRightsSet() const
        {
            return myRightsSet;
        }


        void Identification::removeRights( const RightsSetIterConst& value )
        {
            if ( value != myRightsSet.cend() )
            {
                myRightsSet.erase( value );
            }
        }


        void Identification::addRights( const RightsPtr& value )
        {
            if ( value )
            {
                myRightsSet.push_back( value );
            }
        }


        void Identification::clearRightsSet()
        {
            myRightsSet.clear();
        }


        RightsPtr Identification::getRights( const RightsSetIterConst& setIterator ) const
        {
            if( setIterator != myRightsSet.cend() )
            {
                return *setIterator;
            }
            return RightsPtr();
        }


        EncodingPtr Identification::getEncoding() const
        {
            return myEncoding;
        }


        void Identification::setEncoding( const EncodingPtr& value )
        {
            if( value )
            {
                myEncoding = value;
            }
        }


        bool Identification::getHasEncoding() const
        {
            return myHasEncoding;
        }


        void Identification::setHasEncoding( const bool value )
        {
            myHasEncoding = value;
        }


        SourcePtr Identification::getSource() const
        {
            return mySource;
        }


        void Identification::setSource( const SourcePtr& value )
        {
            if( value )
            {
                mySource = value;
            }
        }


        bool Identification::getHasSource() const
        {
            return myHasSource;
        }


        void Identification::setHasSource( const bool value )
        {
            myHasSource = value;
        }


        const RelationSet& Identification::getRelationSet() const
        {
            return myRelationSet;
        }


        void Identification::removeRelation( const RelationSetIterConst& value )
        {
            if ( value != myRelationSet.cend() )
            {
                myRelationSet.erase( value );
            }
        }


        void Identification::addRelation( const RelationPtr& value )
        {
            if ( value )
            {
                myRelationSet.push_back( value );
            }
        }


        void Identification::clearRelationSet()
        {
            myRelationSet.clear();
        }


        RelationPtr Identification::getRelation( const RelationSetIterConst& setIterator ) const
        {
            if( setIterator != myRelationSet.cend() )
            {
                return *setIterator;
            }
            return RelationPtr();
        }


        MiscellaneousPtr Identification::getMiscellaneous() const
        {
            return myMiscellaneous;
        }


        void Identification::setMiscellaneous( const MiscellaneousPtr& value )
        {
            if( value )
            {
                myMiscellaneous = value;
            }
        }


        bool Identification::getHasMiscellaneous() const
        {
            return myHasMiscellaneous;
        }


        void Identification::setHasMiscellaneous( const bool value )
        {
            myHasMiscellaneous = value;
        }


        bool Identification::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "creator", myCreatorSet );
                importElementSet( message, it, endIter, isSuccess, "rights", myRightsSet );
                if ( importElement( message, *it, isSuccess, *myEncoding, myHasEncoding ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *mySource, myHasSource ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "relation", myRelationSet );
                if ( importElement( message, *it, isSuccess, *myMiscellaneous, myHasMiscellaneous ) ) { continue; }
            }

            return isSuccess;
        }

    }
}
