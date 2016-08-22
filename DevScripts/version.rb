lines = []

File.open("../Sourcecode/mx/core/AttributesInterface.h", "r") do |input_file|
    input_file.each_line do |line|
    	lines << line
	end
end

rx = /\/\/\sMusicXML\sClass\sLibrary\sv0\.1\.1/
is_first_line_correct = ( ( lines[0] =~ rx ) != nil )

rx = /\/\/\sCopyright\s\(c\)\s2015\s\-\s2016\sby\sMatthew\sJames\sBriggs/
is_second_line_correct = ( ( lines[1] =~ rx ) != nil )

if ( !is_first_line_correct ) || ( !is_second_line_correct )
	puts "filename"
end