// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Creator )
        MX_FORWARD_DECLARE_ELEMENT( Encoding )
        MX_FORWARD_DECLARE_ELEMENT( Miscellaneous )
        MX_FORWARD_DECLARE_ELEMENT( Relation )
        MX_FORWARD_DECLARE_ELEMENT( Rights )
        MX_FORWARD_DECLARE_ELEMENT( Source )
        MX_FORWARD_DECLARE_ELEMENT( Identification )

        inline IdentificationPtr makeIdentification() { return std::make_shared<Identification>(); }

        class Identification : public ElementInterface
        {
        public:
            Identification();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Creator minOccurs = 0, maxOccurs = unbounded _________ */
            const CreatorSet& getCreatorSet() const;
            void addCreator( const CreatorPtr& value );
            void removeCreator( const CreatorSetIterConst& value );
            void clearCreatorSet();
            CreatorPtr getCreator( const CreatorSetIterConst& setIterator ) const;

            /* _________ Rights minOccurs = 0, maxOccurs = unbounded _________ */
            const RightsSet& getRightsSet() const;
            void addRights( const RightsPtr& value );
            void removeRights( const RightsSetIterConst& value );
            void clearRightsSet();
            RightsPtr getRights( const RightsSetIterConst& setIterator ) const;

            /* _________ Encoding minOccurs = 0, maxOccurs = 1 _________ */
            EncodingPtr getEncoding() const;
            void setEncoding( const EncodingPtr& value );
            bool getHasEncoding() const;
            void setHasEncoding( const bool value );

            /* _________ Source minOccurs = 0, maxOccurs = 1 _________ */
            SourcePtr getSource() const;
            void setSource( const SourcePtr& value );
            bool getHasSource() const;
            void setHasSource( const bool value );

            /* _________ Relation minOccurs = 0, maxOccurs = unbounded _________ */
            const RelationSet& getRelationSet() const;
            void addRelation( const RelationPtr& value );
            void removeRelation( const RelationSetIterConst& value );
            void clearRelationSet();
            RelationPtr getRelation( const RelationSetIterConst& setIterator ) const;

            /* _________ Miscellaneous minOccurs = 0, maxOccurs = 1 _________ */
            MiscellaneousPtr getMiscellaneous() const;
            void setMiscellaneous( const MiscellaneousPtr& value );
            bool getHasMiscellaneous() const;
            void setHasMiscellaneous( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            CreatorSet myCreatorSet;
            RightsSet myRightsSet;
            EncodingPtr myEncoding;
            bool myHasEncoding;
            SourcePtr mySource;
            bool myHasSource;
            RelationSet myRelationSet;
            MiscellaneousPtr myMiscellaneous;
            bool myHasMiscellaneous;
        };
    }
}
