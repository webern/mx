// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Date.h"
#include <sstream>

namespace
{
    std::ostream& zeroPad( int value, int width, std::ostream& os )
    {
        char prev_fillch = os.fill ('0');
        std::streamsize prev_width = os.width( width );
        os << value;
        os.fill( prev_fillch );
        os.width( prev_width );
        return os;
    }
}

namespace mx
{
    namespace core
    {
        class Date::impl
        {
        public:
            impl( int year_in, int month_in, int day_in )
            :myYear( 1900 )
            ,myMonth( 1 )
            ,myDay( 1 )
            {
                setYear( year_in );
                setMonth( month_in );
                setDay( day_in );
            }
            
            impl()
            :myYear( 1900 )
            ,myMonth( 1 )
            ,myDay( 1 )
            {}
            
            impl( const std::string& yyyy_mm_dd )
            :myYear( 1900 )
            ,myMonth( 1 )
            ,myDay( 1 )
            { parseString( yyyy_mm_dd ); }
            
            ~impl() {}
        public:
            void setYear( int value )
            {
                myYear = constrainToRange( value, 1, 9999 );
                setDay( getDay() );
            }
            void setMonth( int value )
            {
                myMonth = constrainToRange( value, 1, 12 );
                setDay( getDay() );
            }
            void setDay( int value )
            {
                myDay = constrainToRange( value, 1, daysInMonth( getMonth(), getYear() ) );
            }
            
            bool setValue( int year_in, int month_in, int day_in )
            {
                if ( isDateValid( year_in, month_in, day_in ) )
                {
                    setYear( year_in );
                    setMonth( month_in );
                    setDay( day_in );
                    return true;
                }
                return false;
            }
            bool isLeapYear( int year_in ) const
            {
                /* per Wikipedia
                 
                 Pseudocode to determine whether a year is a leap year
                 or not in either the Gregorian calendar since 1582 or
                 in the proleptic Gregorian calendar between 1 and 1582:
                 
                 if year is divisible by 400 then
                 is_leap_year
                 else if year is divisible by 100 then
                 not_leap_year
                 else if year is divisible by 4 then
                 is_leap_year
                 else
                 not_leap_year
                 
                 */
                if( year_in < 1 || year_in > 9999 )  // error, unacceptable year
                    return false;
                else if ( year_in % 400 == 0 ) // is leapyear per wikipedia
                    return true;
                else if ( year_in % 100 == 0 ) // is NOT a leapyear per wikipedia
                    return false;
                else if ( year_in % 4 == 0 ) // is leapyear per wikipedia
                    return true;
                else
                    return false;   // is not leapyear per wikipedia
            }
            
            bool parse( const std::string value_in )
            {
                return parseString( value_in );
            }
            
            bool parseString( const std::string& value_in )
            {
                if ( isStringFormatValid( value_in ) )
                {
                    int year = -1;
                    int month = -1;
                    int day = -1;
                    if (     parseStringYear( value_in, year )
                        && parseStringMonth( value_in, month )
                        && parseStringDay( value_in, day ) )
                    {
                        if ( day <= daysInMonth( month, year ) )
                        {
                            setValue( year, month, day );
                            return true;
                        }
                    }
                }
                return false;
            } // parseString
            
            bool parseStringYear( const std::string& value_in, int& year_out )
            {
                year_out = -1;
                std::stringstream( value_in.substr( 0, 4 ) ) >> year_out;
                bool success = false;
                if ( year_out >= 1 && year_out <= 9999 )
                {
                    success = true;
                }
                return success;
            } //parseStringYear
            
            bool parseStringMonth( const std::string& value_in, int& month_out )
            {
                month_out = -1;
                std::stringstream( value_in.substr( 5, 2 ) ) >> month_out;
                bool success = false;
                if ( month_out >= 1 && month_out <= 12 )
                {
                    success = true;
                }
                return success;
            } //parseStringMonth
            
            bool parseStringDay( const std::string& value_in, int& day_out )
            {
                day_out = -1;
                std::stringstream( value_in.substr( 8, 2 ) ) >> day_out;
                bool success = false;
                if ( day_out >= 1 && day_out <= 31 )
                {
                    success = true;
                }
                return success;
            } //parseStringDay
            
            bool isStringFormatValid( const std::string& value_in ) const
            {
                int length = (int)value_in.size();
                if ( length != 10 )
                {
                    return false;
                }
                else
                {
                    for ( size_t i = 0; i < static_cast<size_t>( length ); ++i )
                    {
                        if ( ( i <= 3 ) && ! isdigit( value_in[i] ) )
                        {
                            return false;
                        }
                        else if ( ( i >= 5 && i <= 6 ) && ! isdigit( value_in[i] ) )
                        {
                            return false;
                        }
                        else if ( ( i >= 8 && i <= 9 ) && ! isdigit( value_in[i] ) )
                        {
                            return false;
                        }
                        else if ( ( i == 4 || i == 7 ) && ! ( value_in[i] == '-' ) )
                        {
                            return false;
                        }
                    }
                }
                return true;
            }
            
            int daysInMonth( int month_in, int year_in ) const
            {
                if ( isLeapYear( year_in ) )
                {
                    return MONTHDAYS[1][constrainToRange( month_in, 1, 12 )];
                }
                else
                {
                    return MONTHDAYS[0][constrainToRange( month_in, 1, 12 )];
                }
            }
            int constrainToRange( int value_in, int minimumValue, int maximumValue ) const
            {
                if ( value_in < minimumValue )
                {
                    return minimumValue;
                }
                if ( value_in > maximumValue )
                {
                    return maximumValue;
                }
                return value_in;
            }
            int getYear() const { return myYear; }
            int getDay() const { return myDay; }
            int getMonth() const { return myMonth; }
            int getWeekday() const
            {
                // returns sequential day of the week 1 for Sunday, 2 for Monday ... 7 for Saturday
                // this implements Zeller's Congruence
                // algorithm: http://en.wikipedia.org/wiki/Zeller%27s_congruence
                // implementation: http://ideone.com/CkDGvY
                int h,q,m,k,j,day,month,year;
                day = getDay();
                month = getMonth();
                year = getYear();
                if(month==1)
                {
                    month = 13;
                    year--;
                }
                if(month==2)
                {
                    month = 14;
                    year--;
                }
                q = day;
                m = month;
                k = year % 100;
                j = year / 100;
                h = q + 13*(m+1)/5 + k + k/4 + j/4 + 5*j;
                h = h % 7;
                
                // Zeller's Congruence returns 0 for Saturday
                // Convert this to 7 so that
                // 1 = Sunday
                // 2 = Monday
                // 3 = Tuesday
                // 4 = Wednesday
                // 5 = Thursday
                // 6 = Friday
                // 7 = Saturday
                if (h==0)
                {
                    h=7;
                }
                return h;
            }
            
            bool isDateValid( int year_in, int month_in, int day_in ) const
            {
                if ( year_in < 1 )
                {
                    return false;
                }
                else if ( year_in > 9999 )
                {
                    return false;
                }
                else if ( month_in < 1 )
                {
                    return false;
                }
                else if ( month_in > 12 )
                {
                    return false;
                }
                else if ( day_in < 1 )
                {
                    return false;
                }
                else if ( day_in > daysInMonth( month_in, year_in ) )
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            
            
            bool equal( const Date& other_in ) const
            {
                return compare( other_in ) == 0;
            }
            bool notEqual( const Date& other_in ) const
            {
                return compare( other_in ) != 0;
            }
            
            bool greaterThan( const Date& other_in ) const
            {
                int temp = 999;
                temp = compare( other_in );
                return temp > 0;
            }
            bool greaterThanOrEqual( const Date& other_in ) const
            {
                return compare( other_in ) >= 0;
            }
            bool lessThan( const Date& other_in ) const
            {
                return compare( other_in ) < 0;
            }
            bool lessThanOrEqual( const Date& other_in ) const
            {
                return compare( other_in ) <= 0;
            }
            
            void addDays( int days )
            {
                if (days == 0)
                {
                    return;
                }
                else if ( days > 0 )
                {
                    for ( int i = 0; i < days; i++)
                    {
                        if ( equal( Date( 9999, 12, 31 ) ) )
                        {
                            return;
                        }
                        else if ( getMonth() == 12 && getDay() == 31 )
                        {
                            setYear( getYear() + 1 );
                            setMonth( 1 );
                            setDay( 1 );
                        }
                        else if ( getDay() == daysInMonth( getMonth(), getYear() ) )
                        {
                            setMonth( getMonth() + 1 );
                            setDay( 1 );
                        }
                        else
                        {
                            setDay( getDay() + 1 );
                        }
                    }
                }
                else if ( days < 0 )
                {
                    for ( int i = 0; i > days; i-- )
                    {
                        if ( equal( Date( 1, 1, 1 ) ) )
                        {
                            return;
                        }
                        else if ( getMonth() == 1 && getDay() == 1 )
                        {
                            setYear( getYear() - 1 );
                            setMonth( 12 );
                            setDay( 31 );
                        }
                        else if ( getDay() == 1 )
                        {
                            setMonth( getMonth() - 1 );
                            setDay( daysInMonth( getMonth(), getYear() ) );
                        }
                        else
                        {
                            setDay( getDay() - 1 );
                        }
                    }
                }
            }
            void addMonths( int months )
            {
                
                // modulus math is hard to understand when negative numbers are involved
                // store the sign of 'months' as an int and make months positive
                int direction = 1;
                if (months < 0)
                {
                    direction = -1;
                    months *= -1;
                }
                
                // remember what day value we had to start with
                int day_was = getDay();
                
                // figure out how many years to add
                // note: int/int truncates
                int new_year = getYear() + ( direction * ( months / 12 ) );
                // if the year goes out of bounds, return min/max date
                
                // figure out how many months to add
                int new_month = getMonth() + ( direction * ( months % 12 ) );
                
                // if new_month equals 0 then direction must be -1
                // the actual month should be December of the previous year.
                if ( new_month == 0 )
                {
                    --new_year;
                    setMonth( 12 );
                }
                // if new_month < 0 then direction must be -1
                // and, we know we didn't subtract more than -11
                // so we can figure out the correct month by
                // adding 12 and moving to the previous year
                else if ( new_month < 0 )
                {
                    --new_year;
                    setMonth( new_month + 12);
                }
                // if new_month > 12, we know we did not add more than 11
                // so we can add a year and take the mod of new_month;
                else if ( new_month > 12 )
                {
                    ++new_year;
                    setMonth( new_month % 12 );
                }
                else
                {
                    setMonth( new_month );
                }
                
                //  does the day fit within the month?
                int daysover = day_was - daysInMonth( getMonth(), new_year );
                
                //  If the day is out of bounds we need to correct it.
                //  This implements the rules followed by Microsoft Excel 2010
                //  and also by Google Docs (as of 2013) and Sql Server 2012
                //  if the target date has a day value to high for the target
                //  month, then adjust the day value to the maximum day value
                //  available in the target month
                if ( daysover > 0 )
                {
                    // jump to the last day of the month
                    setYear( new_year );
                    setDay( daysInMonth( getMonth(), getYear() ) );
                }
                else
                {
                    setYear( new_year );
                    setDay( day_was ); // restore the correct day value
                }
                
                // check for out-of-bounds year value
                if ( new_year < 1 )
                {
                    setYear( 1 );
                    setMonth( 1 );
                    setDay( 1 );
                }
                else if ( new_year > 9999 )
                {
                    setYear( 9999 );
                    setMonth( 12 );
                    setDay( 31 );
                }
                else
                {
                    setYear( new_year );
                }
            }
            
            void addYears( int years )
            {
                int new_year = getYear() + years;
                //year( ( year() + years ) );
                
                // if day was a leap day and the new year is not a leap year
                // month/day need to be moved to March 1 instead of Feb 29
                if ( getMonth() == 2 && getDay() == 29 && !isLeapYear( new_year ) )
                {
                    setDay( 1 );
                    setMonth( 3 );
                }
                setYear( new_year );
            }

        private:
            int myYear;
            int myMonth;
            int myDay;
            void setInitialValue( int year_in, int month_in, int day_in )
            {
                if ( isDateValid( year_in, month_in, day_in ) )
                {
                    setValue( year_in, month_in, day_in );
                }
                else
                {
                    setValue( 1900, 1, 1 );
                }
            }
            
            int compare( const Date& other ) const
            {
                if ( this->getYear() > other.getYear() )
                {
                    return 1;
                }
                else if ( this->getYear() < other.getYear() )
                {
                    return -1;
                }
                else
                {
                    if ( this->getMonth() > other.getMonth() )
                    {
                        return 1;
                    }
                    else if ( this->getMonth() < other.getMonth() )
                    {
                        return -1;
                    }
                    else
                    {
                        if ( this->getDay() > other.getDay() )
                        {
                            return 1;
                        }
                        else if ( this->getDay() < other.getDay() )
                        {
                            return -1;
                        }
                    }
                }
                return 0;
            }
            
        public:
            const static std::string myXmlTypeName;
            const static std::string myClassName;
            const static std::string myDocumentation;
            const static int myXsdID;
            const static int MONTHDAYS [2][13];
            
            
        }; // struct Date::impl
        

        const int Date::impl::MONTHDAYS [2][13] =
        {
            {0,31,28,31,30,31,30,31,31,30,31,30,31} // 0 = Not a leap year
            ,
            {0,31,29,31,30,31,30,31,31,30,31,30,31} // 1 = Leap year
        };
        
        /* Date ---------------------------------------------------------------- */
        
        /* Constructor, Destructor, Copy, Assignment ------------------------------------------ */
        
        Date::Date( int year_in, int month_in, int day_in )
        :myImpl( new impl( year_in, month_in, day_in ) )
        {}
        
        Date::Date()
        :myImpl( new impl() )
        {}
        
        Date::Date( const std::string& yyyy_mm_dd )
        :myImpl( new impl( yyyy_mm_dd ) )
        {}
        
        Date::~Date() {}
        
        Date::Date( const Date& other )
        :myImpl( new impl( other.myImpl->getYear(),
                          other.myImpl->getMonth(),
                          other.myImpl->getDay() ) )
        {}
        
        
        Date::Date( Date&& other )
        :myImpl( std::move( other.myImpl ) )
        {}
        
        Date& Date::operator=( const Date& other )
        {
            this->myImpl = std::unique_ptr<impl>( new impl( other.myImpl->getYear(),
                                                           other.myImpl->getMonth(),
                                                           other.myImpl->getDay() ) );
            return *this;
        }
        Date& Date::operator=( Date&& other )
        {
            myImpl = std::move( other.myImpl );
            return *this;
        }
        
        /* Getters ---------------------------------------------------------------------------- */
        
        /** Returns the year as an int. **/
        int Date::getYear() const
        {
            return myImpl->getYear();
        }
        
        /** Returns the month as an int. **/
        int Date::getMonth() const
        {
            return myImpl->getMonth();
        }
        
        /** Returns the day as an int. **/
        int Date::getDay() const
        {
            return myImpl->getDay();
        }
        
        /** Returns true if the contained year is a leap year **/
        bool Date::isLeapYear() const
        {
            return myImpl->isLeapYear( getYear() );
        }
        
        /** Returns true if the passed year is a leap year **/
        bool Date::isLeapYear( int year_in ) const
        {
            return myImpl->isLeapYear( year_in );
        }
        
        /** Returns the number of days in the current month **/
        int Date::daysInMonth() const
        {
            return myImpl->daysInMonth( getMonth(), getYear() );
        }
        
        /** Returns the number of days in the passed month/year **/
        int Date::daysIntMonth( int month_in, int year_in ) const
        {
            return myImpl->daysInMonth( month_in, year_in );
        }
        int Date::getWeekday() const
        {
            return myImpl->getWeekday();
        }
        bool Date::setValue( int year_in, int month_in, int day_in )
        {
            return myImpl->setValue( year_in, month_in, day_in );
        }
        void Date::setYear( int year_in )
        {
            myImpl->setYear( year_in );
        }
        void Date::setMonth( int month_in )
        {
            myImpl->setMonth( month_in );
        }
        void Date::setDay( int day_in )
        {
            myImpl->setDay( day_in );
        }
        bool Date::parse( const std::string& value_in )
        {
            return myImpl->parse( value_in );
        }
        bool Date::operator==( const Date& other_in ) const
        {
            return myImpl->equal( other_in );
        }
        bool Date::operator!=( const Date& other_in ) const
        {
            return myImpl->notEqual( other_in );
        }
        bool Date::operator>( const Date& other_in ) const
        {
            return myImpl->greaterThan( other_in );
        }
        bool Date::operator<( const Date& other_in ) const
        {
            return myImpl->lessThan( other_in );
        }
        bool Date::operator>=( const Date& other_in ) const
        {
            return myImpl->greaterThanOrEqual( other_in );
        }
        bool Date::operator<=( const Date& other_in ) const
        {
            return myImpl->lessThanOrEqual( other_in );
        }
        Date& Date::operator++()
        {
            myImpl->addDays( 1 );
            return *this;
        }
        Date Date::operator++( int )
        {
            Date temp( *this );
            myImpl->addDays( 1 );
            return temp;
        }
        Date& Date::operator--()
        {
            myImpl->addDays( -1 );
            return *this;
        }
        Date Date::operator--( int )
        {
            Date temp( *this );
            myImpl->addDays( -1 );
            return temp;
        }
        Date& Date::addDays( int days_in )
        {
            myImpl->addDays( days_in );
            return *this;
        }
        Date& Date::addMonths( int months_in )
        {
            myImpl->addMonths( months_in );
            return *this;
        }
        Date& Date::addYears( int years_in )
        {
            myImpl->addYears( years_in );
            return *this;
        }


        std::string toString( const Date& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }


        std::ostream& toStream( std::ostream& os, const Date& value )
        {
            zeroPad( value.getYear(), 4, os );
            os << "-";
            zeroPad( value.getMonth(), 2, os );
            os << "-";
            zeroPad( value.getDay(), 2, os );
            return os;
        }


        std::ostream& operator<<( std::ostream& os, const Date& value )
        {
            return toStream( os, value );
        }
    }
}
