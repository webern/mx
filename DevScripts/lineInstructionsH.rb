require 'stringio'

require_relative "parseElementsH.rb"

class LineInfo
  attr_accessor :name
  attr_accessor :inherits_from
  attr_accessor :forward_declaration_line
  attr_accessor :make_default_line
  attr_accessor :declaration_line
  attr_accessor :end_line
  
  def initialize
	  @name = ""
	  @inherits_from = ""
	  @forward_declaration_line = -1
	  @make_default_line = -1
	  @declaration_line = -1
	  @end_line = -1
  end
  
  def to_s
    ss = StringIO.new
    ss << "#{name},"
    ss << "#{inherits_from},"
    ss << "#{forward_declaration_line},"
    ss << "#{make_default_line},"
    ss << "#{declaration_line},"
    ss << "#{end_line}"
    ss.string
  end
end

def findDefaultMakeLine line_info, code_lines
	if line_info.inherits_from.eql? "AttributesInterface"
		return
	end
	rx = /make#{line_info.name}\(\)/
	#rx = /.*/
	start = line_info.forward_declaration_line
	stop = line_info.declaration_line
	#puts "start: #{start}"
	#puts "stop: #{stop}"
	for i in start..stop
		line = code_lines[i]
		#puts "i: #{i}"
		#puts "code_lines[i]: #{code_lines[i]}"
		#puts "line: #{line}"
		if ( line =~ rx ) != nil
			#puts "BINGO!!!!!!!!!!!!!!!!!!!!!!!!!!"
            line_info.make_default_line = i
        end
    end
end

def findDefaultMakeLinesForAll line_info_collection, code_lines
	line_info_collection.each do |li|
		findDefaultMakeLine li, code_lines
	end
end

def lineInstructionsH

	lines = []

	File.open("../Sourcecode/mx/core/Elements.h", "r") do |f|
	    f.each_line do |line|
	    	lines << line
		end
	end

	parsed_h = parseElementsH



	temp_line_instructions = []

	line_info = LineInfo.new
	start_flag = false

	parsed_h.each do |i|
		if i.description.eql? "forward_declaration"
			#puts i.description
			line_info.name = i.name
			line_info.forward_declaration_line = i.line_number
			next
		end

		if i.description.eql? "declaration"
			#puts i.description
			line_info.inherits_from = i.inherits_from
			line_info.declaration_line = i.line_number
			next
		end

		#if start_flag
		#	if i.inherits_from != ""
		#		line_info.inherits_from = i.inherits_from
		#	end
		#end

		if i.description.eql? "close_brace"
	#		puts "hello"
			start_flag = false
			line_info.end_line = i.line_number
			temp_line_instructions << line_info
			line_info = LineInfo.new
		end
	end

	final_line_instructions = []

	previous_li = temp_line_instructions[0]

	temp_line_instructions.each_with_index do |li,index|
		if ( li.name.nil? || li.name.eql?( "" ) )
			previous_li.end_line = li.end_line
		else
			final_line_instructions << previous_li
			previous_li = LineInfo.new
			previous_li = li
		end
	end

	final_line_instructions << previous_li

	findDefaultMakeLinesForAll final_line_instructions, lines
	#puts final_line_instructions
	final_line_instructions
end

#puts lineInstructionsH