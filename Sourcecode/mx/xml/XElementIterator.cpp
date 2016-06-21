// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/XElementIterator.h"

namespace mx
{
    namespace xml
    {
        XElementIterator::XElementIterator()
        : myImpl( nullptr )
        {
            
        }
        
        
        XElementIterator::XElementIterator( const XElementIterImpl& impl )
        : myImpl( impl.clone() )
        {

        }
        
        
        XElementIterator::XElementIterator( const XElementIterator& other )
        : myImpl( nullptr )
        {
            if( other.myImpl )
            {
                myImpl = other.myImpl->clone();
            }
        }
        
        
        XElementIterator& XElementIterator::operator=( const XElementIterator& other )
        {
            if( other.myImpl )
            {
                myImpl = other.myImpl->clone();
            }
            return *this;
        }


        bool XElementIterator::operator==(const XElementIterator& rhs) const
        {
            if( !myImpl )
            {
                return false;
            }
            return myImpl->equals( rhs );
        }
        
        
        bool XElementIterator::operator!=( const XElementIterator& rhs ) const
        {
            if( !myImpl )
            {
                return false;
            }
            return !myImpl->equals( rhs );
        }
        
        
        XElement& XElementIterator::operator*() const
        {
            if( !myImpl )
            {
                throw std::runtime_error( "XElementIterator::operator*() - null dereference attempted" );
            }
            return myImpl->getRef();
        }
        
        
        XElement* XElementIterator::operator->() const
        {
            if( !myImpl )
            {
                throw std::runtime_error( "XElementIterator::operator->() - null dereference attempted" );
            }
            return myImpl->getPtr();
        }
        
        
        const XElementIterator& XElementIterator::operator++()
        {
            if( myImpl )
            {
                myImpl->increment();
            }
            return *this;
        }
        
        
        XElementIterator XElementIterator::operator++(int)
        {
            if( !myImpl )
            {
                return XElementIterator{};
            }
            XElementIterImplUP temp = myImpl->clone();
            myImpl->increment();
            return XElementIterator{ *temp };
        }
        
        
        const XElementIterator& XElementIterator::operator--()
        {
            if( myImpl )
            {
                myImpl->decrement();
            }
            return *this;
        }
        
        
        XElementIterator XElementIterator::operator--(int)
        {
            if( !myImpl )
            {
                return XElementIterator{};
            }
            XElementIterImplUP temp = myImpl->clone();
            myImpl->decrement();
            return XElementIterator{ *temp };
        }


        bool XElementIterator::getIsPayloadNull() const
        {
            if( !myImpl )
            {
                return true;
            }

            if( myImpl->getIsPayloadNull() )
            {
                return true;
            }

            return false;
        }
        
        
        const XElementIterImplUP& XElementIterator::reveal() const
        {
            return myImpl;
        }
    }
}