require "set"

def listClassesInEnumsH

    enum_list = SortedSet.new

    File.open("../Sourcecode/mx/core/Enums.h", "r") do |f|
        f.each_line do |line|
        	rx = /^\s+class\s([a-zA-Z0-9]+)/
        	matches = line.match(rx)
            if !matches.nil?
                caps = matches.captures
                enum_name = caps[0]
                enum_list << enum_name
                #puts enum_name
            end
    	end
    end

    enum_list

end
