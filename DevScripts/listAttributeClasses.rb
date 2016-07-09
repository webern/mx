require "set"
require_relative "parseElementsH.rb"

class AttributeClass

end


def listAttributeClasses

	elements_result = parseElementsH

	attribute_classes = SortedSet.new

	elements_result.each do |ed|
		if ed.description.eql? "declaration"
			if ed.inherits_from.eql? "AttributesInterface"
				attribute_classes << ed.name
			end
		end
	end

	#attribute_classes << "InvertedMordentAttributes"
	#attribute_classes << "ToeAttributes"
	
	attribute_classes
end


#listAttributeClasses.each do |ac|
#	puts ac
#end
