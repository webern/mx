
require_relative "isSymbolFound.rb"

def includeIfNeeded( symbol_list, code_snippet_to_search, include_file_path )
	return_value = ""
	symbol_list.each do |symbol|
			if isSymbolFound(code_snippet_to_search,symbol)
				return "#include \"#{include_file_path}\"\n"
			end
		end
	return_value
end

def includeOneIfNeeded( symbol, code_snippet_to_search, include_file_path )
	return_value = ""
	if isSymbolFound(code_snippet_to_search,symbol)
		return "#include \"#{include_file_path}\"\n"
	end
	return_value
end