expected_identifier = "// MusicXML Class Library v0.2"
expected_copyright = "// Copyright (c) 2015 - 2016 by Matthew James Briggs"

new_identifier = "// MusicXML Class Library v0.3.0"
new_copyright = "// Copyright (c) 2015 - 2016 by Matthew James Briggs"

def regexify input_string
	input_string.gsub!('/','\/')
	input_string.gsub!(' ','\s')
	input_string.gsub!('(','\(')
	input_string.gsub!(')','\)')
	input_string.gsub!('[','\[')
	input_string.gsub!(']','\]')
	input_string.gsub!('-','\-')
end

regexify expected_identifier
regexify expected_copyright

directories = [
	"../Sourcecode/mx/core",
	"../Sourcecode/mx/core/elements",
	"../Sourcecode/mx/xml",
	"../Sourcecode/mx/utility",	
	"../Sourcecode/mxtest/utility",	
	"../Sourcecode/mxtest/core",	
	"../Sourcecode/mxtest/xml",	
	"../Sourcecode/mxtest/control",	
	"../Sourcecode/mxtest/import"
]

bad_files = []

# CHECK ALL OF THE FILES ################################################################

directories.each do |dir|
	drectory_with_star = "#{dir}/*"
	Dir[drectory_with_star].each do |item|
	next if item == '.' or item == '..' or item == ".DS_Store" or File.directory?(item)
		filename = "#{item}"
		#puts filename
		lines = []

		File.open(filename, "r") do |input_file|
		    input_file.each_line do |line|
		    	lines << line
			end
		end

		rx = /#{expected_identifier}/
		is_first_line_correct = ( ( lines[0] =~ rx ) != nil )

		rx = /#{expected_copyright}/
		is_second_line_correct = ( ( lines[1] =~ rx ) != nil )

		if ( !is_first_line_correct ) || ( !is_second_line_correct )
			bad_files << filename
		end # if

	end # foreach file

end # foreach dir

# ABORT IF UNEXPECTED STRINGS EXIST OUT THERE ##########################################

bad_files.each do |bad_file|
	puts "subl #{bad_file}"
end

if bad_files.length > 0
	abort("Some files do not have the expected copyright lines")
end

# UPDATE FILES #########################################################################

directories.each do |dir|
	drectory_with_star = "#{dir}/*"
	Dir[drectory_with_star].each do |item|
	next if item == '.' or item == '..' or item == ".DS_Store" or File.directory?(item)
		filename = "#{item}"
		#puts filename
		lines = []

		File.open(filename, "r") do |input_file|
		    input_file.each_line do |line|
		    	lines << line
			end
		end

		File.open(filename, "w") do |output_file|
		   
		   lines.each_with_index do |line, index|

		   		if index == 0
		   			output_file << new_identifier << "\n"
		   		elsif index == 1
		   			output_file << new_copyright << "\n"
		   		else
		   			output_file << line
		   		end

			end #lines.each 
		
		end # File.open

		
		

	end # foreach file

end # foreach dir