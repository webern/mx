require 'stringio'

require_relative 'listEnums.rb'
require_relative 'listClassesInEnumsH'
require_relative 'listSpecialClasses'
require_relative 'listDecimals'
require_relative 'listIntegers'
require_relative 'listStrings'

class IntermediateStruct
  attr_accessor :const
  attr_accessor :typename
  attr_accessor :prefix
  attr_accessor :name
  def to_s
    "const: #{const}, typename: #{typename}, prefix: #{prefix}, name: #{name}, xmlname: #{xmlname}"
  end

  def xmlname
	temp = name.split(/([[:upper:]][[:lower:]]*)/).delete_if(&:empty?).join("-").downcase
	temp.gsub('_',' ')
  end
	def initialize
  	@const = false
  	@typename = ""
  	@prefix = ""
  	@name = ""
  end
end


class MemberInfo
  attr_accessor :type
  attr_accessor :name
  attr_accessor :is_required
  attr_accessor :xmlname
  def to_s
    "type: #{type}, name: #{name}, is_required: #{is_required}, xmlname: #{xmlname}"
  end
  def initialize
  	@type = ""
  	@name = ""
  	@is_required = false
  	@xmlname = ""
  end

  def name_capitalized
  	( name.slice(0,1).capitalize + name.slice(1..-1) ).gsub('_',' ')
  end

  def finders_bool
  	if is_required
	    "is#{name_capitalized}Found"
  	else
  		"has#{name_capitalized}"
  	end
  		
  end

  def parsing_statement enums_list #, decimals_list, integers_list, strings_list, classes_in_enumh, specials_list
  	 
  	 if enums_list.include? type
  	 	"if( parseAttribute( message, it, className, isSuccess, #{name}, #{finders_bool}, \"#{xmlname}\", &parse#{type} ) ) { continue; }"

  	 else
  	 	"if( parseAttribute( message, it, className, isSuccess, #{name}, #{finders_bool}, \"#{xmlname}\" ) ) { continue; }" 

  	 end

  end

end


def from_xelement_code indent, enums_list, member_list, class_name
	io = StringIO.new
	io << indent << "bool #{class_name}::fromXElement( std::ostream& message, xml::XElement& xelement )" << "\n"
	io << indent << "{" << "\n"
	io << indent << "    const char* const className = \"#{class_name}\";" << "\n"
	io << indent << "    bool isSuccess = true;" << "\n"
	each_required_bool_init io, indent, enums_list, member_list
	io << indent << "" << "\n"
	io << indent << "    auto it = xelement.attributesBegin();" << "\n"
	io << indent << "    auto endIter = xelement.attributesEnd();" << "\n"
	io << indent << "" << "\n"
	io << indent << "    for( ; it != endIter; ++it )" << "\n"
	io << indent << "    {" << "\n"
	each_iter_loop_code io, indent, enums_list, member_list
	io << indent << "    }" << "\n"
	io << indent << "" << "\n"
	each_required_bool_check io, indent, enums_list, member_list
	io << indent << "" << "\n"
	io << indent << "    return isSuccess;" << "\n"
	io << indent << "}" << "\n"
	io.string
end

def each_iter_loop_code io, indent, enums_list, member_list
	member_list.each do |m|
		io << indent << "        #{m.parsing_statement(enums_list)}" << "\n"
	end
end

def each_required_bool_init io, indent, enums_list, member_list
	member_list.each do |m|
		if m.is_required
			io << indent << "    bool #{m.finders_bool} = false;" << "\n"
		end
	end
end

def each_required_bool_check io, indent, enums_list, member_list
	member_list.each do |m|
		if m.is_required
			io << indent << "    if( !#{m.finders_bool} )" << "\n"
			io << indent << "    {" << "\n"
			io << indent << "        isSuccess = false;" << "\n"
			io << indent << "        message << className << \": 'number' is a required attribute but was not found\" << std::endl;" << "\n"
			io << indent << "    }" << "\n"
		end
	end
end


def implementAttributeImport class_name

	list_enums = listEnums
	#list_classes_in_enums_h = listClassesInEnumsH
	#list_special_classes = listSpecialClasses
	#list_decimals = listDecimals
	#list_integers = listIntegers
	#list_strings = listStrings

	hlines = []
	hlines_last_member = -1
	clines = []

	cline_first_include = -1
	cline_last_function_close = -1

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



	hintermediates = []

	File.open("../Sourcecode/mx/core/elements/#{class_name}.h", "r") do |f|
		#puts caller.first
		rx_public = /        public\:/
		is_member_list = false
		f.each_with_index do |line, index|
			hlines << line
				
			if ( line =~ rx_public ) != nil
				is_member_list = true
				#puts caller.first
				next
			elsif is_member_list
				#puts caller.first
				if ( line =~ /\s+[a-zA-Z0-9\_]+\s+[a-zA-Z0-9\_]+/ ) == nil
						if ( line =~ /#{class_name}/ ) == nil
							#puts line
							#puts caller.first
							is_member_list = false
							hlines_last_member = index
						end
				else 
					#make the list of members here
					rx_const_has = /\s+const\s+bool\s+(has)([a-zA-Z0-9\_]+)/
					rx_has = /\s+bool\s+(has)([a-zA-Z0-9\_]+)/
					rx_other = /\s+([a-zA-Z0-9\_]+)\s+([a-zA-Z0-9\_]+)/

				    #capture const has
					if ( line =~ rx_const_has ) != nil
						matches = line.match(rx_const_has)
				        if !matches.nil?
				            has_capture, name_capture = matches.captures
				            x = IntermediateStruct.new
				            x.const = true
				            x.typename = "bool"
				            x.prefix = has_capture
				            x.name = name_capture
				            hintermediates << x
				        end

					#capture regular has
					elsif ( line =~ rx_has ) != nil
						matches = line.match(rx_has)
				        if !matches.nil?
				            has_capture, name_capture = matches.captures
				            x = IntermediateStruct.new
				            x.const = false
				            x.typename = "bool"
				            x.prefix = has_capture
				            x.name = name_capture
				            hintermediates << x
				        end

				    #capture non bools
					elsif ( line =~ rx_other ) != nil
						matches = line.match(rx_other)
				        if !matches.nil?
				            type_capture, name_capture = matches.captures
				            x = IntermediateStruct.new
				            x.const = false
				            x.typename = type_capture
				            x.prefix = ""
				            x.name = name_capture
				            hintermediates << x
				        end
					end
				end
			end
		end
	end



	hmembers = []

	hintermediates.each do |i|

		if !i.const && (i.typename.eql? "bool") && (i.prefix.eql? "has") && (i.name.eql? "Values")
			next
		end

		if !i.const && (i.typename.eql? "virtual") && (i.prefix.eql? "") && (i.name.eql? "std")
			next
		end

		info = MemberInfo.new
		need_to_add = true
		hmembers.each do |hmem|
			if hmem.name.casecmp(i.name) == 0
				info = hmem
				need_to_add = false
			end
		end

		if i.prefix.eql? "has"
			info.is_required = i.const
		else
			info.name = i.name
			info.type = i.typename
			info.xmlname = i.xmlname
		end


		if need_to_add
			need_to_add = false
     		hmembers << info
		end

	end

hmembers.delete_if { |x| ( x.name == nil ) || ( x.name.eql? "" ) }



	File.open("../Sourcecode/mx/core/elements/#{class_name}.h", "w") do |f|
		hlines.each_with_index do |line, index|
			if( index == hlines_last_member )
				f << "\n"
				f << "            bool fromXElement( std::ostream& message, xml::XElement& xelement );" << "\n"
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
				f << from_xelement_code( "        ", list_enums, hmembers, class_name )
				next
			end # end if( index == cline_last_function_close )
			f << line
		end # end clines.each_with_index do |line, index|
	end # end cfile write

end # end of function

implementAttributeImport "InvertedMordentAttributes"