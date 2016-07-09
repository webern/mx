lines = []

File.open("../Sourcecode/mx/core/Elements.h", "r") do |input_file|
    input_file.each_line do |line|
    	lines << line
	end
end

File.open("../Sourcecode/mx/core/Elements.h.replace", "w") do |f|
	write_line = true
    lines.each do |line|


	    rx = /^((?!\/\*\s\_\_\_).)*$/
	    is_not_a_good_comment = ( line =~ rx ) != nil

	    rx = /\/\*/
	    is_a_comment = ( line =~ rx ) != nil

        if is_not_a_good_comment && is_a_comment
        	write_line = false
    	end

	    if write_line
	    	f << line
	    end

	    rx = /\*\//

        if ( line =~ rx ) != nil
        	write_line = true
    	end

    end
end

