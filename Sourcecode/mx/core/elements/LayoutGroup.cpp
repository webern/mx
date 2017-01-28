// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PageLayout.h"
#include "mx/core/elements/StaffLayout.h"
#include "mx/core/elements/SystemLayout.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        LayoutGroup::LayoutGroup()
        :myPageLayout( makePageLayout() )
        ,myHasPageLayout( false )
        ,mySystemLayout( makeSystemLayout() )
        ,myHasSystemLayout( false )
        ,myStaffLayoutSet()
        {}


        bool LayoutGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& LayoutGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& LayoutGroup::streamName( std::ostream& os ) const
        {
            os << "work";
            return os;
        }


        bool LayoutGroup::hasContents() const
        {
            return myHasPageLayout || myHasSystemLayout || myStaffLayoutSet.size() > 0;
        }


        std::ostream& LayoutGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                bool isFirst = true;
                if ( myHasPageLayout )
                {
                    myPageLayout->toStream( os, indentLevel );
                    isFirst = false;
                }
                if ( myHasSystemLayout )
                {
                    if ( !isFirst )
                    {
                        os << std::endl;
                        isFirst = false;
                    }
                    mySystemLayout->toStream( os, indentLevel );
                }
                if ( myStaffLayoutSet.size() > 0 )
                {
                    for ( auto it = myStaffLayoutSet.cbegin();
                         it != myStaffLayoutSet.cend(); ++it )
                    {
                        if ( it == myStaffLayoutSet.cbegin() )
                        {
                            if ( !isFirst )
                            {
                                os << std::endl;
                                isFirst = false;
                            }
                        }
                        else
                        {
                            os << std::endl;
                        }
                        (*it)->toStream( os, indentLevel );
                    }
                }
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PageLayoutPtr LayoutGroup::getPageLayout() const
        {
            return myPageLayout;
        }


        void LayoutGroup::setPageLayout( const PageLayoutPtr& value )
        {
            if ( value )
            {
                myPageLayout = value;
            }
        }


        bool LayoutGroup::getHasPageLayout() const
        {
            return myHasPageLayout;
        }


        void LayoutGroup::setHasPageLayout( const bool value )
        {
            myHasPageLayout = value;
        }


        SystemLayoutPtr LayoutGroup::getSystemLayout() const
        {
            return mySystemLayout;
        }


        void LayoutGroup::setSystemLayout( const SystemLayoutPtr& value )
        {
            if ( value )
            {
                mySystemLayout = value;
            }
        }


        bool LayoutGroup::getHasSystemLayout() const
        {
            return myHasSystemLayout;
        }


        void LayoutGroup::setHasSystemLayout( const bool value )
        {
            myHasSystemLayout = value;
        }


        const StaffLayoutSet& LayoutGroup::getStaffLayoutSet() const
        {
            return myStaffLayoutSet;
        }


        void LayoutGroup::addStaffLayout( const StaffLayoutPtr& value )
        {
            if ( value )
            {
                myStaffLayoutSet.push_back( value );
            }
        }


        void LayoutGroup::removeStaffLayout( const StaffLayoutSetIterConst& value )
        {
            if ( value != myStaffLayoutSet.cend() )
            {
                myStaffLayoutSet.erase( value );
            }
        }


        void LayoutGroup::clearStaffLayoutSet()
        {
            myStaffLayoutSet.clear();
        }


        StaffLayoutPtr LayoutGroup::getStaffLayout( const StaffLayoutSetIterConst& setIterator ) const
        {
            if( setIterator != myStaffLayoutSet.cend() )
            {
                return *setIterator;
            }
            return StaffLayoutPtr();
        }
        
        
        MX_FROM_XELEMENT_UNUSED( LayoutGroup );

    }
}
