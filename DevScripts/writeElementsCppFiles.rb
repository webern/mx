require 'stringio'
require 'set'

require_relative "parseElementsCpp.rb"
require_relative "listElementClasses.rb"
require_relative "listAttributeClasses.rb"
require_relative "lineInstructionsH.rb"
require_relative "includeIfNeeded.rb"

def doIncludes io_object, line_info_object
	if line_info_object == nil
		puts "ERROR line_info_object is nil"
		return
	end
	element_classes = listElementClasses
	lines = []

	File.open("../Sourcecode/mx/core/Elements.h", "r") do |f|
    	f.each_line do |line|
    		lines << line
		end
	end

	code_stringio = StringIO.new
	for i in line_info_object.forward_declaration_line..line_info_object.end_line
		code_stringio << "#{lines[i]}"
	end
	code = code_stringio.string


	element_classes.each do |e|
		if !e.eql? line_info_object.name
			io_object << includeOneIfNeeded( "#{e}Ptr", code, "mx/core/elements/#{e}.h" )
		end
	end

	io_object << "#include <iostream>\n"
end

def openNamespaces io_object
	io_object << "namespace mx\n"
	io_object << "{\n"
	io_object << "    namespace core\n"
	io_object << "    {\n"
end

def closeNamespaces io_object
	io_object << "    }\n"
	io_object << "}\n"
end

def emptyLine io_object
	io_object << "\n"
end

def includeH io_object, class_name
	io_object << "#include \"mx/core/elements/#{class_name}.h\"\n"
end

def copywrite io_object
	io_object << "// MusicXML Class Library v0.1.1\n"
	io_object << "// Copyright (c) 2015 - 2016 by Matthew James Briggs\n"
end

def writeCppFile class_name, original_file_lines, function_objects

	line_info_objects = lineInstructionsH
	line_info_selected = line_info_objects.select { |li| li.name.eql? class_name }
	
	if line_info_selected.size == 0
		puts "ERROR line_info_object not found for #{class_name}"
	end

	line_info = line_info_selected[0]

	File.open("../Sourcecode/mx/core/elements/#{class_name}.cpp", "w") do |f|
		copywrite f
		emptyLine f
		includeH( f, class_name )
		doIncludes( f, line_info )
		emptyLine f
		openNamespaces f
		
		# functions
		function_objects.each do |func|
			original_file_lines[func.first_line..func.last_line].each do |line|
				f << line
			end
			emptyLine f
			emptyLine f
		end
		
		
		closeNamespaces f
	end
end


lines = []

File.open("../Sourcecode/mx/core/Elements.cpp", "r") do |f|
    f.each_line do |line|
    	lines << line
	end
end

classes = listElementClasses
attributes = listAttributeClasses
all_symbols = SortedSet.new
all_symbols.merge classes
all_symbols.merge attributes

all_functions = parseElementsCpp

all_symbols.each do |c|
	local_functions = all_functions.select { |func| func.class_name.eql? c }
	writeCppFile c, lines, local_functions
end




#puts all_functions.count

#writeCppFile "temp", lines, functions