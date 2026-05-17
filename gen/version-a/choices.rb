require "stringio"

enum_value_string = 
'

                multipleRest = 0,
                measureRepeat = 1,
                beatRepeat = 2,
                slash = 3


'

enum_symbols = []

enum_value_string.lines.each do |line|


	rx = /\s+([a-zA-Z0-9\_]+)\s+/
	if ( line =~ rx ) != nil

	    matches = line.match(rx)

	    if !matches.nil?
	        caps = matches.captures
	        enum_symbol = caps[0]
	        enum_symbols << enum_symbol	        

	    end

	end

end

class Enum
  attr_accessor :symbol
  def to_s
    "symbol: #{symbol}, element: #{element}, caps: #{caps},"
  end

  def initialize in_symbol
  	@symbol = in_symbol
  end

  def element
  	clean = remove_trailing_underscore
  	dashed = clean.split(/([[:upper:]][[:lower:]]*)/).delete_if(&:empty?).join("-")
  	dashed.downcase
  end

  def caps
  	clean = remove_trailing_underscore
  	clean.slice(0,1).capitalize + clean.slice(1..-1)
  end

  def remove_trailing_underscore
  	rx = /([a-zA-Z0-9]+)/

	if ( symbol =~ rx ) != nil

	    matches = symbol.match(rx)

	    if !matches.nil?
	        caps = matches.captures
	        without_underscore = caps[0]
	        return without_underscore       

	    end

	end

  end
end


puts "        bool DisplayTextOrAccidentalText::fromXElement( std::ostream& message, xml::XElement& xelement )"
puts "        {"
enum_symbols.each do |es|
e = Enum.new es
puts "            MX_CHOICE_IF( #{e.symbol}, \"#{e.element}\", #{e.caps} );"
end
puts "            MX_BAD_ELEMENT_FAILURE( CLASSNAME );"
puts "        }"

