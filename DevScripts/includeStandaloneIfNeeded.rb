require "set"
require_relative "isSymbolFound.rb"

def includeStandaloneIfNeeded( symbol, code_snippet_to_search )
	return_value = ""
	if isSymbolFound(code_snippet_to_search,symbol)
		return "#include \"mx/core/#{symbol}.h\"\n"
	end
	if isSymbolFound(code_snippet_to_search,"#{symbol}Ptr")
		return "#include \"mx/core/#{symbol}.h\"\n"
	end
	if isSymbolFound(code_snippet_to_search,"#{symbol}Set")
		return "#include \"mx/core/#{symbol}.h\"\n"
	end
	return_value
end