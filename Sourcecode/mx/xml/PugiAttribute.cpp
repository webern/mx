// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/PugiAttribute.h"
#include "mx/xml/PugiAttributeIterImpl.h"
#include "mx/xml/XDoc.h"

namespace mx
{
    namespace xml
    {
        PugiAttribute::PugiAttribute()
        : myAttribute()
        , myParentElement()
        , myXDoc()
        {

        }


        PugiAttribute::PugiAttribute(
            const pugi::xml_attribute& attribute,
            const pugi::xml_node& parentElement,
            const XDocCPtr& parentXDoc )
        : myAttribute( attribute )
        , myParentElement( parentElement )
        , myXDoc( parentXDoc )
        {

        }


        bool PugiAttribute::getIsNull() const
        {
            auto ptr = myXDoc.lock();

            if( !ptr )
            {
                return true;
            }

            if( myParentElement.type() != pugi::node_element )
            {
                return true;
            }

            return false;
        }


        std::string PugiAttribute::getName() const
        {
            return myAttribute.name();
        }


        std::string PugiAttribute::getValue() const
        {
            return myAttribute.value();
        }


        void PugiAttribute::setName( const std::string& name )
        {
            myAttribute.set_name( name.c_str() );
        }


        void PugiAttribute::setValue( const std::string& value )
        {
            myAttribute.set_value( value.c_str() );
        }

    }
}