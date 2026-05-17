
require 'set'



def listSpecialClasses

	list = SortedSet.new

	list << "Color"
	list << "Date"
	list << "FontSize"
	list << "NumberOrNormal"
	list << "PositiveIntegerOrEmpty"
	list << "YesNoNumber"

	return list
end

#puts listSpecialClasses