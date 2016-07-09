
require 'stringio'

class String
  def uncapitalize 
    self[0, 1].downcase + self[1..-1]
  end
end

class Member
  attr_accessor :my_variable_name
  attr_accessor :element_name
  attr_accessor :is_set
  attr_accessor :is_required
  attr_accessor :element_name_for_cpp

  def initialize in_my_variable_name, in_element_name, in_description, in_element_name_for_cpp
	  @my_variable_name = in_my_variable_name
	  @element_name = in_element_name
	  @element_name_for_cpp = in_element_name_for_cpp

	  if in_description.eql? "collection"
	  	@is_set = true
	  	@is_required = false
	  elsif in_description.eql? "required"
	  	@is_set = false
	  	@is_required = true
	  elsif in_description.eql? "optional"
	  	@is_set = false
	  	@is_required = false
	  else
	  	@is_set = false
	  	@is_required = false
	  end
  end

  def is_found_variable
  	"is#{element_name_for_cpp}Found"
  end

  def is_first_added_variable
  	"isFirst#{element_name_for_cpp}Added"
  end

end

=begin
example_required_single = Member.new "myCreditWords", "credit-words", "required", "CreditWords"
example_optional_single = Member.new "myOptionalThing", "optional-thing", "optional", "OptionalThing"
example_collection = Member.new "myCatSet", "cat", "collection", "Cat"
=end

group_class_name = "MetronomeRelationGroup"

members = []

#sample code
members << Member.new( "myMetronomeRelation", "metronome-relation", "required", "MetronomeRelation" )
members << Member.new( "myMetronomeNote", "metronome-note", "required", "MetronomeNote" )

code = StringIO.new
tab = "    "
code << tab << tab << "bool #{group_class_name}::fromXElement( std::ostream& message, xml::XElement& xelement )" << "\n"
code << tab << tab << "{" << "\n"
code << tab << tab << tab << "bool isSuccess = true;" << "\n"

members.each do |m|
	if m.is_required && !m.is_set
		code << tab << tab << tab << "bool #{m.is_found_variable} = false;" << "\n"
	elsif m.is_set
		code << tab << tab << tab << "bool #{m.is_first_added_variable} = false;" << "\n"
	end
end
code << "\n"
code << tab << tab << tab << "for( auto it = xelement.elementsBegin(); it != xelement.elementsEnd(); ++it )" << "\n"
code << tab << tab << tab << "{" << "\n"
code << tab << tab << tab << tab << "const std::string elementName = it->getName();" << "\n"
code << "\n"
first_if = true
members.each do |m|
	
	code << tab << tab << tab << tab 	
	if !first_if
		code << "else "
	end
	first_if = false
	code << "if( elementName == \"#{m.element_name}\" )" << "\n"
	code << tab << tab << tab << tab << "{" << "\n"
	if m.is_set
code << "                    auto #{m.element_name_for_cpp.uncapitalize} = make#{m.element_name_for_cpp}();" << "\n"
code << "                    isSuccess &= #{m.element_name_for_cpp.uncapitalize}->fromXElement( message, *it );" << "\n"
code << "\n"
code << "                    if( !#{m.is_first_added_variable} && #{m.my_variable_name}.size() == 1 )" << "\n"
code << "                    {" << "\n"
code << "                        *( #{m.my_variable_name}.begin() ) = #{m.element_name_for_cpp.uncapitalize};" << "\n"
code << "                        #{m.is_first_added_variable} = true;" << "\n"
code << "                    }" << "\n"
code << "                    else" << "\n"
code << "                    {" << "\n"
code << "                        #{m.my_variable_name}.push_back( #{m.element_name_for_cpp.uncapitalize} );" << "\n"
code << "                        #{m.is_first_added_variable} = true;" << "\n"
code << "                    }" << "\n"
	elsif m.is_required
code << "                    #{m.is_found_variable} = true;" << "\n"
code << "                    isSuccess &= #{m.my_variable_name}->fromXElement( message, *it );" << "\n"
	else
code << "                    myHas#{m.element_name_for_cpp} = true;" << "\n"
code << "                    isSuccess &= #{m.my_variable_name}->fromXElement( message, *it );" << "\n"
	end
	code << tab << tab << tab << tab << "}" << "\n"
end
code << tab << tab << tab << tab << "else" << "\n"
code << tab << tab << tab << tab << "{" << "\n"

members.each do |m|
	if m.is_required
code << "                    if( !#{m.is_found_variable} )" << "\n"
code << "                    {" << "\n"
code << "                        isSuccess = false;" << "\n"
code << "                        message << \"#{group_class_name}: '#{m.element_name}' element is required but was not found\" << std::endl;" << "\n"
code << "                    }" << "\n"
	end
end
code << "                    break;" << "\n"
code << tab << tab << tab << tab << "}" << "\n"
code << tab << tab << tab << "}" << "\n"
code << tab << tab << tab << "return isSuccess;" << "\n"
code << tab << tab << "}" << "\n"

puts code.string