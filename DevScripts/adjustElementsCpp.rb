lines = []

File.open("../Sourcecode/mx/core/Elements.cpp", "r") do |input_file|
    input_file.each_line do |line|
    	lines << line
	end
end

File.open("../Sourcecode/mx/core/Elements.cpp.replace", "w") do |f|
	write_line = true
    lines.each do |line|

	    rx = /^\s*$/
	    skip_line = ( line =~ rx ) != nil


	    if !skip_line
	    	f << line
	    end



    end
end

