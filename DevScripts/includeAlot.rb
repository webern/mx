require 'pathname'

filepaths = Dir["../Sourcecode/mx/core/elements/*"]

filepaths.each do |fp|
	filename = Pathname.new(fp).basename
	classname = File.basename( fp, ".*" )
	puts "#include \"mx/core/elements/#{classname}.h\""
end
