        /// A base class for integer types
        class Integer
        {
        public:
            virtual ~Integer() = default;
            virtual IntType getValue() const = 0;
            virtual void setValue( IntType value ) = 0;
            virtual void parse( const std::string& value ) = 0;
        };


        Integer::Integer()
        : Integer{ 0 }
        {
        
        }
        
        Integer::Integer( IntType value )
        : myValue{}
        {
            // for child classes that clamp the range
            setValue( 0 );
        }
        
        IntType Integer::getValue() const
        {
            return myValue;
        }
        
        void setValue( IntType value )
        {
            myValue = value;
        }

        void Integer::parse( const std::string& value )
        {
            std::stringstream ss( value );
            IntType temp = 0;
            if ((ss >> temp).fail() || !(ss >> std::ws).eof())
                return;
            }
            setValue( temp );
        }