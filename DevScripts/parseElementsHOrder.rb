lines = []

File.open("../Sourcecode/mx/core/Elements.h", "r") do |input_file|
    input_file.each_line do |line|
    	lines << line
	end
end

i = 1
#File.open("../Sourcecode/mx/core/Elements.h.order.csv", "w") do |f|
	write_line = true
    lines.each do |line|
    	
    	rx = /\s{8}class\s([a-zA-Z0-9\_]+)\s\:/
    	matches = line.match(rx)
	    
	    if !matches.nil?
	        caps = matches.captures
	        name = caps[0]
	        f << "#{i},#{name},ElementInterface\n"
	        i = (i + 1)
	        next
	    end

	    rx = /\s{8}struct\s([a-zA-Z0-9\_]+)\s\:/
    	matches = line.match(rx)
	    
	    if !matches.nil?
	        caps = matches.captures
	        name = caps[0]
	        f << "#{i},#{name},AttributesInterface\n"
	        i = (i + 1)
	        next
	    end

    end
end