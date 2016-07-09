require_relative 'attributes.rb'
require 'pathname'
paths = Dir["../Sourcecode/mx/core/elements/*.h"]

names = []

paths.each do |path|
	names << File.basename( path, ".*" )
end

attributes_classes = []

names.each do |name|
	rx = /[a-zA-Z0-9\_]+Attributes/
	if ( name =~ rx ) != nil
		attributes_classes << name
	end
end

attributes_classes.each do |ac|
	implementAttributeImport ac
end