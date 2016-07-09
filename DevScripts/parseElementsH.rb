require 'stringio'
require 'set'

class HDeclarationLine
  attr_accessor :line_number
  attr_accessor :name
  attr_accessor :object_type
  attr_accessor :inherits_from
  attr_accessor :description
  def to_s
    ss = StringIO.new
    ss << "#{line_number},"
    ss << "#{name},"
    ss << "#{object_type},"
    ss << "#{inherits_from},"
    ss << "#{description}"
    ss.string
  end
end

def parseElementsH
    declarations_found = []

    File.open("../Sourcecode/mx/core/Elements.h", "r") do |f|
      f.each_with_index do |line, index|
        rx = /^\s*class\s[^\s\;]+/
        is_class = ( line =~ rx ) != nil

        rx = /^\s*struct\s[^\s\;]+/
        is_struct = ( line =~ rx ) != nil
            
        object_type = "struct"
        if is_class
            object_type = "class"
        end

        is_forward_dec = false

        rx = /^\s.*class.*\;.*$/
        if ( line =~ rx ) != nil
            is_forward_dec = true
        end

        rx = /^\s.*struct.*\;.*$/
        if ( line =~ rx ) != nil
            is_forward_dec = true
        end

        rx = /^\s*class\s([^\s\;]+)/
        name = ""
        matches = line.match(rx)
        if !matches.nil?
            caps = matches.captures
            name = caps[0]
            #puts name
        end

        rx = /^\s*struct\s([^\s\;]+)/
        matches = line.match(rx)
        if !matches.nil?
            caps = matches.captures
            name = caps[0]
            #puts name
        end

        rx = /\:\s+public\s+([a-zA-Z]+)/
        inherits = ""
        is_decl = false
        if is_class || is_struct
            if !is_forward_dec
                matches = line.match(rx)
                if !matches.nil?
                    caps = matches.captures
                    inherits = caps[0]
                    is_decl = true
                end
            end
        end

        if is_class || is_struct
            addthis = HDeclarationLine.new
            addthis.line_number = index
            addthis.name = name
            addthis.object_type = object_type
            addthis.inherits_from = inherits
            if is_forward_dec
                addthis.description = "forward_declaration"
            elsif is_decl
                addthis.description = "declaration"
            end
                
            declarations_found << addthis
        end

        if !is_class && !is_struct
            rx = /^\s+\{/
            if ( line =~ rx ) != nil
                addthis = HDeclarationLine.new
                addthis.line_number = index
                addthis.name = ""
                addthis.object_type = ""
                addthis.inherits_from = ""
                addthis.description = "open_brace"
                declarations_found << addthis
            end
        end

        if !is_class && !is_struct
            rx = /^\s+\}\;/
            if ( line =~ rx ) != nil
                addthis = HDeclarationLine.new
                addthis.line_number = index
                addthis.name = ""
                addthis.object_type = ""
                addthis.inherits_from = ""
                addthis.description = "close_brace"
                declarations_found << addthis
            end
        end

      end
    end

    declarations_found
end


#stuff = parseElementsH
#puts stuff
