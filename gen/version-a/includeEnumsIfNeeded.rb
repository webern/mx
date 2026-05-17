
require_relative "isSymbolFound.rb"

def includeEnumsIfNeeded( enum_symbols, enum_class_symbols, code_snippet_to_search )
	return_value = ""
	enum_symbols.each do |enum_symbol|
		if isSymbolFound(code_snippet_to_search,enum_symbol)
			return "#include \"mx/core/Enums.h\"\n"
		end
	end
	enum_class_symbols.each do |enum_class_symbol|
		if isSymbolFound(code_snippet_to_search,enum_class_symbol)
			return "#include \"mx/core/Enums.h\"\n"
		end
	end
	return_value
end