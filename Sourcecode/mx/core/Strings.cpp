// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/Strings.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iterator>

namespace
{
    const mx::core::StringType kWhitespace = " \t\n\v\f\r";
    const mx::core::StringType kNCNameAllowed = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
    mx::core::StringType trim(const mx::core::StringType& str,
                     const mx::core::StringType& whitespace = kWhitespace )
    {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == mx::core::StringType::npos)
            return ""; // no content
        
        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;
        
        return str.substr(strBegin, strRange);
    }
    
    mx::core::StringType reduce(const mx::core::StringType& str,
                       const mx::core::StringType& fill = " ",
                       const mx::core::StringType& whitespace = kWhitespace)
    {
        // trim first
        auto result = trim(str, whitespace);
        
        // replace sub ranges
        auto beginSpace = result.find_first_of(whitespace);
        while (beginSpace != mx::core::StringType::npos)
        {
            const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
            const auto range = endSpace - beginSpace;
            
            result.replace(beginSpace, range, fill);
            
            const auto newStart = beginSpace + fill.length();
            beginSpace = result.find_first_of(whitespace, newStart);
        }
        
        return result;
    }
    mx::core::StringType onlyAllow(const mx::core::StringType& input,
                                    const mx::core::StringType& fill = "_",
                                    const mx::core::StringType& onlyAllow = kNCNameAllowed)
    {
        std::string str = input;
        auto firstBadChar = str.find_first_not_of( onlyAllow );
        while ( firstBadChar != mx::core::StringType::npos )
        {
            str = str.replace( firstBadChar, 1, fill );
            firstBadChar = str.find_first_not_of( onlyAllow );
        }
        return str;
    }
    
    std::set<int> copyPositives( const std::set<int>& input )
    {
        std::set<int> output;
        std::copy_if( input.cbegin(), input.cend(), std::inserter( output, output.begin() ),
                     [](const int i){ return i > 0; } );
        return output;
    }
}

namespace mx
{
    namespace core
    {
        XsString::XsString()
        :myValue( "" ) {}
        
        XsString::XsString( const StringType& value )
        :myValue( value ) {}
        
        XsString::~XsString() {}
        
        StringType XsString::getValue() const
        {
            return myValue;
        }
        void XsString::setValue( const StringType& value )
        {
            myValue = value;
        }
        StringType toString( const XsString& xsstring )
        {
            return xsstring.getValue();
        }
        std::ostream& toStream( std::ostream& os, const XsString& xsstring )
        {
            return os << xsstring.getValue();
        }
        std::ostream& operator<<( std::ostream& os, const XsString& xsstring )
        {
            return toStream( os, xsstring );
        }
        
        XsToken::XsToken()
        :XsString() {}
        
        XsToken::XsToken( const StringType& value )
        :XsString( value )
        {
            XsToken::setValue( value );
        }
        void XsToken::setValue( const StringType& value )
        {
            XsString::setValue( reduce( value ) );
        }
        
        XsID::XsID()
        :XsString( "ID" ) {}
        
        XsID::XsID( const StringType& value )
        :XsString( value )
        {
            XsID::setValue( value );
        }
        
        void XsID::setValue( const StringType& value )
        {
            std::string scrubbed = onlyAllow( value );
            if ( scrubbed.length() == 0 )
            {
                scrubbed = "ID";
            }
            else if ( isdigit( scrubbed[0] ) )
            {
                scrubbed = "ID" + scrubbed;
            }
            XsString::setValue( scrubbed );
        }
        
        CommaSeparatedText::CommaSeparatedText()
        :myValues() {}
        
        CommaSeparatedText::CommaSeparatedText( const XsTokenSet& values )
        :myValues( values ) {}
        
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
        CommaSeparatedListOfPositiveIntegers::CommaSeparatedListOfPositiveIntegers()
        :myValues() {}
        CommaSeparatedListOfPositiveIntegers::CommaSeparatedListOfPositiveIntegers( const StringType& value )
        :myValues()
        {
            parse( value );
        }
        CommaSeparatedListOfPositiveIntegers::CommaSeparatedListOfPositiveIntegers( const std::set<int>& values )
        :myValues()
        {
            setValues( values );
        }
        const std::set<int>& CommaSeparatedListOfPositiveIntegers::getValues() const
        {
            return myValues;
        }
        std::set<int>::iterator CommaSeparatedListOfPositiveIntegers::getValuesBegin()
        {
            return myValues.begin();
        }
        std::set<int>::iterator CommaSeparatedListOfPositiveIntegers::getValuesEnd()
        {
            return myValues.end();
        }
        std::set<int>::const_iterator CommaSeparatedListOfPositiveIntegers::getValuesBeginConst() const
        {
            return myValues.cbegin();
        }
        std::set<int>::const_iterator CommaSeparatedListOfPositiveIntegers::getValuesEndConst() const
        {
            return myValues.cend();
        }
        void CommaSeparatedListOfPositiveIntegers::setValues( const std::set<int>& values )
        {
            myValues = copyPositives( values );
        }
        void CommaSeparatedListOfPositiveIntegers::parse( const StringType& commaSeparatedText )
        {
            StringType cleaned = onlyAllow( commaSeparatedText, "", "1234567890,-" );
            myValues.clear();
            std::istringstream iss( cleaned );
            std::string token;
            while( std::getline( iss, token, ',' ) )
            {
                if ( token.length() > 0 )
                {
                    std::stringstream ss( token );
                    int value = 1;
                    ss >> value;
                    if ( value > 0 )
                    {
                        myValues.insert( value );
                    }
                }
            }
        }
        std::ostream& toStream( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value )
        {
            bool isfirst = true;
            for ( auto x : value.getValues() )
            {
                if( isfirst )
                {
                    os << x;
                    isfirst = false;
                }
                else
                {
                    os << ",";
                    os << x;
                }
            }
            return os;
        }
        std::ostream& operator<<( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value )
        {
            return toStream( os, value );
        }
        StringType toString( const CommaSeparatedListOfPositiveIntegers& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
    }
}