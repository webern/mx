require "set"

def listIntegers

    item_list = SortedSet.new

    File.open("../Sourcecode/mx/core/Integers.h", "r") do |f|
        f.each_line do |line|
        	rx = /^\s+class\s([a-zA-Z0-9]+)/
        	matches = line.match(rx)
            if !matches.nil?
                caps = matches.captures
                enum_name = caps[0]
                item_list << enum_name
                #puts enum_name
            end

            rx = /^\s+using\s([a-zA-Z0-9]+)/
            matches = line.match(rx)
            if !matches.nil?
                caps = matches.captures
                enum_name = caps[0]
                item_list << enum_name
                #puts enum_name
            end

    	end
    end

    #item_list.delete("impl")
    #item_list << "kDefaultPrecision"
    #item_list << "kNonZeroAmount"
    item_list
end


#listIntegers.each do |d|
#	puts d
#end