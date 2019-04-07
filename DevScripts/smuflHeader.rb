require 'json'
require 'stringio'

j = JSON.parse( IO.read("../Resources/S-M-U-F-L/glyphnames.json"))

content = StringIO.new

content << "// MusicXML Class Library v0.3.0\n"
content << "// Copyright (c) 2015 - 2016 by Matthew James Briggs\n"
content << "\n"
content << "#pragma once\n"
content << "\n"
content << "#include <string>\n"
content << "#include <map>\n"
content << "\n"
content << "namespace mx\n"
content << "{\n"
content << "    namespace impl\n"
content << "    {\n"
content << "        inline std::map<const std::string, const char16_t> createS-M-U-F-LMap()\n"
content << "        {\n"
content << "            std::map<const std::string, const char16_t> outMap;\n"

#content << "            outMap.emplace(std::make_pair<const std::string, const char16_t>( "4stringTabClef", 0xE06E ));\n"


j.each_with_index do |item,index|
	content << "            "
	content << "outMap.emplace(std::make_pair<const std::string, const char16_t>( \"#{item[0]}\", "
	content << "0x"
	content << item[1]["codepoint"][2..-1]
	content << " )); // glyphnumber: #{index}, #{item[1]["description"]}"
	content << "\n"
	#puts index
	#puts item[0]
	#puts item[1]["codepoint"]
	#puts item[1]["description"]
end

content << "            return outMap;\n"
content << "        }\n"
content << "    }\n"
content << "}\n"

begin
  file = File.open("../Sourcecode/mx/impl/S-M-U-F-LGlyphMap.h", "w")
  file.write(content.string) 
rescue IOError => e
  #some error occur, dir not writable etc.
ensure
  file.close unless file.nil?
end
