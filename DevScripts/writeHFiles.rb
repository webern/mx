require 'stringio'

require_relative "lineInstructionsH.rb"
require_relative "listEnums.rb"
require_relative "listClassesInEnumsH.rb"
require_relative "listStrings.rb"
require_relative "listDecimals.rb"
require_relative "listIntegers.rb"
require_relative "includeEnumsIfNeeded.rb"
require_relative "includeIfNeeded.rb"
require_relative "includeStandaloneIfNeeded.rb"
require_relative "listAttributeClasses.rb"
require_relative "listElementClasses.rb"
require_relative "elementsHUsingFile.rb"


#def forwardDeclare class_name
#	"        MX_FORWARD_DECLARE( #{class_name} )\n"
#end

def forwardDeclareAttributesIfNeeded( io_object, symbol_name, code )
	if (!isSymbolFound( code , "#{symbol_name}Ptr" ))&&(!isSymbolFound( code , "#{symbol_name}Set" ))&&(!isSymbolFound( code , symbol_name ))
		return
	end

	#old way
	#io_object << "        struct #{symbol_name};\n"
	#io_object << "        using #{symbol_name}Ptr = std::shared_ptr<#{symbol_name}>;\n"

	#new way
	io_object << "        MX_FORWARD_DECLARE_ATTRIBUTES( #{symbol_name} )\n"

	#io_object << "\n"
end

def forwardDeclareElementIfNeeded( io_object, symbol_name, code )
	if (!isSymbolFound( code , "#{symbol_name}Ptr" ))&&(!isSymbolFound( code , "#{symbol_name}Set" ))&&(!isSymbolFound( code , symbol_name ))
		return
	end

	#old way
	#io_object << "        class #{symbol_name};\n"
	#io_object << "        using #{symbol_name}Ptr = std::shared_ptr<#{symbol_name}>;\n"
	#io_object << "        using #{symbol_name}Set = std::vector<#{symbol_name}Ptr>;\n"
	#io_object << "        using #{symbol_name}SetIter = #{symbol_name}Set::iterator;\n"
	#io_object << "        using #{symbol_name}SetIterConst = #{symbol_name}Set::const_iterator;\n"
	
	#new way
	io_object << "        MX_FORWARD_DECLARE_ELEMENT( #{symbol_name} )\n"
	#io_object << "\n"
end

def getCodeAsString line_instruction, h_file_code_lines
	code_stringio = StringIO.new
	stop = line_instruction.end_line
	start = line_instruction.declaration_line

	first_make = line_instruction.make_default_line

	if ( first_make > 0 )
		if ( first_make ) < start
			start = ( first_make )
		end
	end

	for i in start..stop
		code_stringio << "#{h_file_code_lines[i]}"
	end
	
	code_stringio.string
end
#puts lineInstructionsH

lines = []

File.open("../Sourcecode/mx/core/Elements.h", "r") do |f|
    f.each_line do |line|
    	lines << line
	end
end




lineInstructionsH.each do |li|
	File.open("../Sourcecode/mx/core/elements/#{li.name}.h", "w") do |f|     
		f << "// MusicXML Class Library v0.1.1" << "\n"
		f << "// Copyright (c) 2015 - 2016 by Matthew James Briggs" << "\n"
		f << "" << "\n"
		f << "#pragma once" << "\n"
		f << "" << "\n"
		f << "#include \"mx/core/ForwardDeclare.h\"" << "\n"
		
		#includes
		if li.inherits_from.eql? "AttributesInterface"
			f << "#include \"mx/core/AttributesInterface.h\"" << "\n"
		elsif li.inherits_from.eql? "ElementInterface"
			f << "#include \"mx/core/ElementInterface.h\"" << "\n"
		end
				
		#store the core code lines for reference
		code = getCodeAsString li, lines

		decimals_symbols = listDecimals
		f << includeIfNeeded( decimals_symbols, code, "mx/core/Decimals.h" )
		
		enum_symbols = listEnums
		enums_h_classes = listClassesInEnumsH
		f << includeEnumsIfNeeded( enum_symbols, enums_h_classes, code )

		integers_symbols = listIntegers
		f << includeIfNeeded( integers_symbols, code, "mx/core/Integers.h" )
		
		strings_symbols = listStrings
		f << includeIfNeeded( strings_symbols, code, "mx/core/Strings.h" )




		f << includeStandaloneIfNeeded( "Color", code )
		f << includeStandaloneIfNeeded( "Date", code )
		f << includeStandaloneIfNeeded( "Document", code )
		f << includeStandaloneIfNeeded( "DocumentPartwise", code )
		f << includeStandaloneIfNeeded( "DocumentTimewise", code )
		f << includeStandaloneIfNeeded( "DocumentHeader", code )
		f << includeStandaloneIfNeeded( "DocumentSpec", code )
		f << includeStandaloneIfNeeded( "EmptyPrintObjectStyleAlignAttributes", code )
		f << includeStandaloneIfNeeded( "FontSize", code )
		f << includeStandaloneIfNeeded( "NumberOrNormal", code )
		f << includeStandaloneIfNeeded( "PositiveIntegerOrEmpty", code )
		f << includeStandaloneIfNeeded( "YesNoNumber", code )


		attribute_classes = listAttributeClasses
		attribute_classes.each do |a|
			f << includeOneIfNeeded( "#{a}Ptr", code, "mx/core/elements/#{a}.h" )
		end

		f << "\n"
		f << "#include <iosfwd>\n"
		f << "#include <memory>\n"
		f << "#include <vector>\n"


		#element_classes = listElementClasses
		#element_classes.each do |e|
		#	if !e.eql? li.name
		#		f << includeOneIfNeeded( "#{e}Ptr", code, "mx/core/elements/#{e}.h" )
		#	end
		#end
		

		f << "" << "\n"
		f << "namespace mx" << "\n"
		f << "{" << "\n"
		f << "    namespace core" << "\n"
		f << "    {" << "\n"
		f << "" << "\n"

		#forward declarations
		attribute_classes = listAttributeClasses
		if !li.inherits_from.eql? "AttributesInterface"
			attribute_classes.each do |a|
				if !a.eql? li.name
					forwardDeclareAttributesIfNeeded( f, a, code )
				end
			end
		end

		element_classes = listElementClasses
		element_classes.each do |e|
			if !e.eql? li.name
				forwardDeclareElementIfNeeded( f, e, code )
			end
		end

		if li.inherits_from.eql? "ElementInterface"
			forwardDeclareElementIfNeeded( f, li.name, code )
		end

		if li.inherits_from.eql? "AttributesInterface"
			forwardDeclareAttributesIfNeeded( f, li.name, code )
		end

		f << "" << "\n"
		f << code
		#for i in li.declaration_line..li.end_line
		#	f << "#{lines[i]}"
		#end

		f << "    }" << "\n"
		f << "}" << "\n"
	end
end

# one off stupid files for using statements
#elementsHUsingFile( "EditorialVoiceDirectionGroup", "EditorialVoiceGroup" )
#elementsHUsingFile( "InvertedMordentAttributes", "MordentAttributes" )
#elementsHUsingFile( "ToeAttributes", "HeelAttributes" )
