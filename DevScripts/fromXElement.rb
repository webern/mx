require_relative "listEnums.rb"

class_name = ""

hlines = []
clines = []
hmembers = []

hline_private = -1
cline_first_include = -1
cline_last_function_close = -1

class Member
  attr_accessor :member_type
  attr_accessor :member_name
  def to_s
    "member_type: #{member_type}, member_name: #{member_name}"
  end
end

File.open("../Sourcecode/mx/core/elements/#{class_name}.h", "r") do |f|

	rx_private = /        private\:/
	f.each_with_index do |line, index|
		hlines << line
		if ( line =~ rx_private ) != nil
			hline_private = index
		end

		#parse the members
		if hline_private > 0
			rx_member = /            ([a-zA-Z0-9]+) ([a-zA-Z0-9]+)/
			if ( line =~ rx_member ) != nil
				matches = line.match(rx_member)
		        if !matches.nil?
		            member_type, member_name = matches.captures
		            m = Member.new
		            m.member_name = member_name
		            m.member_type = member_type
		            hmembers << m
		        end
			end
		end
	end

	if hline_private < 0
		hline_private = hlines.length - 5
	end
end

#test
#something = Member.new
#something.member_type = "bool"
#something.member_name = "myHasCreditChoice"
#hmembers << something

def is_optional name, member_list
	member_list.each do |m|
		if m.member_name.eql? "myHas#{name[2..999]}"

			return true
		end
	end
	return false
end

def is_empty_with_attributes member_list
	if member_list.length == 1
		if member_list[0].member_name.eql? "myAttributes"
			return true
		end
	end
	return false
end

def is_set member_type
	rx = /.+Set/
	return ( member_type =~ rx ) != nil
end

def is_group member_type
	rx = /.+GroupSet/	
	if ( member_type =~ rx ) != nil
		return true
	end 

	rx = /.+GroupPtr/	
	if ( member_type =~ rx ) != nil
		return true
	end 
	return false
end

def is_choice member_type
	rx = /.+ChoiceSet/	
	if ( member_type =~ rx ) != nil
		return true
	end 

	rx = /.+ChoicePtr/	
	if ( member_type =~ rx ) != nil
		return true
	end 
	return false
end

def has_attributes member_list
	member_list.each do |m|
		if m.member_name.eql? "myAttributes"
			return true
		end
	end
	return false
end

def is_bool member
	return member.member_type.eql? "bool"
end

def guts io, member_list, class_name

io << "            bool isSuccess = true;\n"
				if has_attributes member_list
io << "            isSuccess &= myAttributes->fromXElement( message, xelement );\n"
				end
			member_list.each do |m|
				if is_bool m
					next
				elsif is_group m.member_type
					next
				elsif is_choice m.member_type
					next
				elsif is_set m.member_type
					next
				elsif m.member_name.eql? "myAttributes"
					next
				elsif !is_optional m.member_name, member_list
io << "            bool is#{m.member_name[2..999]}Found = false;\n"
				end
			end
io << "\n"
io << "            auto endIter = xelement.elementsEnd();\n"
io << "            for( auto it = xelement.elementsBegin(); it != endIter; ++it )\n"
io << "            {\n"
			member_list.each do |m|
				if is_bool m
					next
				elsif is_group m.member_type
io << "                // groups #{m.member_name} \n\n"
				elsif is_choice m.member_type
io << "\n                /* if( checkSetChoiceMember(\n"
io << "                    message, *it, isSuccess, #{m.member_name},\n"
io << "                    \"elementname\", &#{m.member_name[2..999]}::getElementName,\n"
io << "                    static_cast<int>( #{m.member_name[2..999]}::Choice::elementname ) ) )\n"
io << "                { is#{m.member_name[2..999]}Found == true; continue; } */\n"

				elsif is_set m.member_type
io << "                importElementSet( message, it, endIter, isSuccess, \"elementstring\", #{m.member_name} );\n"
				elsif m.member_name.eql? "myAttributes"
					next
				elsif !is_optional m.member_name, member_list
io << "                if ( importElement( message, *it, isSuccess, *#{m.member_name}, is#{m.member_name[2..999]}Found ) ) { continue; }\n"
				else
io << "                if ( importElement( message, *it, isSuccess, *#{m.member_name}, myHas#{m.member_name[2..999]} ) ) { continue; }\n"
				end
			end
io << "            }\n"
io << "\n"


			member_list.each do |m|
				if is_bool m
					next
				elsif is_group m.member_type
					next
				elsif is_choice m.member_type
					next
				elsif is_set m.member_type
					next
				elsif m.member_name.eql? "myAttributes"
					next
				elsif !is_optional m.member_name, member_list
io << "            if( !is#{m.member_name[2..999]}Found )\n"
io << "            {\n"
io << "                message << \"#{class_name}: '\" << #{m.member_name}->getElementName() << \"' is required but was not found\" << std::endl;\n"
io << "            }\n"
				end
			end



io << "            return isSuccess;\n"
end

def guts_empty_with_attributes io
	io << "            return myAttributes->fromXElement( message, xelement );\n"
end

def is_value_with_attributes member_list
	if member_list.length == 2
		if member_list[0].member_name.eql? "myAttributes"
			if member_list[1].member_name.eql? "myValue"
				return true
			end
		end

		if member_list[0].member_name.eql? "myValue"
			if member_list[1].member_name.eql? "myAttributes"
				return true
			end
		end

	end
	return false
end

def is_value_without_attributes member_list

	if member_list.length == 1

		if member_list[0].member_name.eql? "myValue"
			return true
		end

	end
	return false
end



def is_empty_element member_list
	if member_list.length == 0
		return true
	end
	return false
end

def guts_empty_element io
	io << "            return true;\n"
end

def setvalue_vs_parse member
	rx = /Xs/
	if ( member.member_type =~ rx ) != nil
		return "myValue.setValue"
	elsif listEnums.include? member.member_type
		return "myValue = parse#{member.member_type}"
	end
	return "myValue.parse"
end

def guts_value_with_attributes io, members
	io << "            bool isSuccess = true;\n"
    io << "            isSuccess &= myAttributes->fromXElement( message, xelement );\n"
    io << "            #{setvalue_vs_parse(members[0])}( xelement.getValue() );\n"
    io << "            return isSuccess;\n"
end

def guts_value_without_attributes io, members
    io << "            #{setvalue_vs_parse(members[0])}( xelement.getValue() );\n"
    io << "            return true;\n"
end

File.open("../Sourcecode/mx/core/elements/#{class_name}.cpp", "r") do |f|
	is_first_include_found = false
	rx_first_include = /\#include/
	rx_function_close = /        \}/
	
	f.each_with_index do |line, index|
		clines << line
	
		if !is_first_include_found
			if ( line =~ rx_first_include ) != nil
				cline_first_include = index
				is_first_include_found = true
			end
		end

		if ( line =~ rx_function_close ) != nil
			cline_last_function_close = index
		end
	end
end


File.open("../Sourcecode/mx/core/elements/#{class_name}.h", "w") do |f|
	hlines.each_with_index do |line, index|
		if( index == hline_private )
			f << "            bool fromXElement( std::ostream& message, xml::XElement& xelement );" << "\n"
			f << "\n"
		end
		f << line
	end
end


File.open("../Sourcecode/mx/core/elements/#{class_name}.cpp", "w") do |f|
	clines.each_with_index do |line, index|
		if( index == cline_first_include )
			f << line
			f << "\#include \"mx/core/FromXElement.h\"" << "\n"
			next
		end

		if( index == cline_last_function_close )
			f << line
			f << "\n\n"
f << "        bool #{class_name}::fromXElement( std::ostream& message, xml::XElement& xelement )\n"
f << "        {\n"

		if is_empty_with_attributes hmembers

			guts_empty_with_attributes f


		elsif is_value_with_attributes hmembers

			guts_value_with_attributes f, hmembers


		elsif is_empty_element hmembers

			guts_empty_element f


		elsif is_value_without_attributes hmembers

			guts_value_without_attributes f, hmembers


		else

			guts f, hmembers, class_name

		end

f << "        }\n"



			next
		end # end if( index == cline_last_function_close )
		f << line
	end # end clines.each_with_index do |line, index|
end # end cfile write
