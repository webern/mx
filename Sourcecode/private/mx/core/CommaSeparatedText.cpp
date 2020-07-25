// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/CommaSeparatedText.h"

#include <istream>
#include <sstream>

namespace mx
{
    namespace core
    {
        CommaSeparatedText::CommaSeparatedText()
        :myValues()
        {

        }
        
        
        CommaSeparatedText::CommaSeparatedText( const XsTokenSet& values )
        :myValues( values )
        {

        }
        
        
        CommaSeparatedText::CommaSeparatedText( const StringType& value )
        :myValues()
        {
            this->parse( value );
        }
        
        
        void CommaSeparatedText::parse( const StringType& commaSeparatedText )
        {
            myValues.clear();
            std::istringstream iss( commaSeparatedText );
            std::string token;
            
            while( std::getline( iss, token, ',' ) )
            {
                myValues.push_back( XsToken( token ) );
            }
        }
        
        
        const XsTokenSet& CommaSeparatedText::getValues() const
        {
            return myValues;
        }
        
        
        void CommaSeparatedText::setValues( const XsTokenSet& values )
        {
            myValues = values;
        }


        void CommaSeparatedText::addValue( const XsToken& value )
        {
            myValues.emplace_back( value );
        }
        
        
        XsTokenSetIter CommaSeparatedText::getValuesBegin()
        {
            return myValues.begin();
        }
        
        
        XsTokenSetIter CommaSeparatedText::getValuesEnd()
        {
            return myValues.end();
        }
        
        
        XsTokenSetIterConst CommaSeparatedText::getValuesBeginConst() const
        {
            return myValues.cbegin();
        }
        
        
        XsTokenSetIterConst CommaSeparatedText::getValuesEndConst() const
        {
            return myValues.cend();
        }
        
        
        std::ostream& toStream( std::ostream& os, const CommaSeparatedText& value )
        {
            bool isfirst = true;
            for ( auto x : value.getValues() )
            {
                if( isfirst )
                {
                    toStream( os, x );
                    isfirst = false;
                }
                else
                {
                    os << ",";
                    toStream( os, x );
                }
            }
            return os;
        }


        std::ostream& operator<<( std::ostream& os, const CommaSeparatedText& value )
        {
            return toStream( os, value );
        }
        
        
        StringType toString( const CommaSeparatedText& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
    }
}
