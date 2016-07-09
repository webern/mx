require "set"
require_relative "parseElementsH.rb"

class ElementClass

end


def listElementClasses

	elements_result = parseElementsH

	element_classes = SortedSet.new

	elements_result.each do |ed|
		if ed.description.eql? "declaration"
			if ed.inherits_from.eql? "ElementInterface"
				element_classes << ed.name
			end
		end
	end

	element_classes << "EditorialVoiceDirectionGroup"

	element_classes
end


#listElementClasses.each do |ec|
#	puts ec
#end
