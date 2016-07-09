
require "stringio"
require "set"
require_relative "listElementClasses.rb"
require_relative "listAttributeClasses.rb"

class CppFunction
  attr_accessor :class_name
  attr_accessor :func_name
  attr_accessor :first_line
  attr_accessor :last_line
  def to_s
    ss = StringIO.new
    ss << "#{class_name},"
    ss << "#{func_name},"
    ss << "#{first_line},"
    ss << "#{last_line}"
    ss.string
  end
end

def parseElementsCpp
    element_classes = listElementClasses
    attribute_classes = listAttributeClasses
    all_classes = SortedSet.new
    all_classes.merge element_classes
    all_classes.merge attribute_classes


    cpp_functions = []

    File.open("../Sourcecode/mx/core/Elements.cpp", "r") do |f|
      f.each_with_index do |line, index|
        
        rx_scope_operator = /([a-zA-Z0-9]+)\:\:([a-zA-Z0-9]+)/
        rx_scope_operator_matches = line.match(rx_scope_operator)
        if rx_scope_operator_matches.nil?
            next
        end

        if line.include? "//"
            next
        end
        
        class_name, func_name = rx_scope_operator_matches.captures

        if class_name.nil? || class_name.empty?
            next
        end

        if func_name.nil? || func_name.empty?
            next
        end

        if class_name.eql? "std"
            next
        end

        if class_name.eql? "core"
            next
        end

        if all_classes.member? class_name
            func = CppFunction.new
            func.first_line = index
            func.class_name = class_name
            func.func_name = func_name
            func.last_line = -1
            cpp_functions << func
        end
      end

      previous_cpp_function = CppFunction.new
      current_cpp_function = CppFunction.new

      cpp_functions.each do |f|
        current_cpp_function = f
        if !previous_cpp_function.first_line.nil?
            previous_cpp_function.last_line = ( current_cpp_function.first_line - 1 )
        end
        previous_cpp_function = current_cpp_function
      end

      previous_cpp_function.last_line = 37340
      
      cpp_functions
    end
end

=begin

cpp_functions = parseElementsCpp
classes = listElementClasses


classes.each do |c|
    puts "-------------------"
    local_functions = cpp_functions.select { |func| func.class_name.eql? c }
    local_functions.each do |f|
    puts f
    end
end
=end
