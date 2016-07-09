

def elementsHUsingFile( alias_symbol, actual_symbol )
	# one off stupid files for using statements
	File.open("../Sourcecode/mx/core/elements/#{alias_symbol}.h", "w") do |stupid_one_off_file|
		stupid_one_off_file << "// MusicXML Class Library v0.1.1" << "\n"
		stupid_one_off_file << "// Copyright (c) 2015 - 2016 by Matthew James Briggs" << "\n"
		stupid_one_off_file << "" << "\n"
		stupid_one_off_file << "#pragma once" << "\n"
		stupid_one_off_file << "#include \"mx/core/elements/#{actual_symbol}.h\"" << "\n"
		stupid_one_off_file << "" << "\n"
		stupid_one_off_file << "namespace mx" << "\n"
		stupid_one_off_file << "{" << "\n"
		stupid_one_off_file << "    namespace core" << "\n"
		stupid_one_off_file << "    {" << "\n"
		stupid_one_off_file << "" << "\n"
		stupid_one_off_file << "        using #{alias_symbol} = #{actual_symbol};\n"
	    stupid_one_off_file << "        using #{alias_symbol}Ptr = std::shared_ptr<#{alias_symbol}>;\n"
	    stupid_one_off_file << "        using #{alias_symbol}UPtr = std::unique_ptr<#{alias_symbol}>;\n"
	    stupid_one_off_file << "        using #{alias_symbol}Set = std::vector<#{alias_symbol}Ptr>;\n"
	    stupid_one_off_file << "        using #{alias_symbol}SetIter = #{alias_symbol}Set::iterator;\n"
	    stupid_one_off_file << "        using #{alias_symbol}SetIterConst = #{alias_symbol}Set::const_iterator;\n"
	    stupid_one_off_file << "        inline #{alias_symbol}Ptr make#{alias_symbol}() { return std::make_shared<#{alias_symbol}>(); }\n"
	    stupid_one_off_file << "    }" << "\n"
		stupid_one_off_file << "}" << "\n"
    end

end