
require_relative "isSymbolFound.rb"

def includeAttributesIfNeeded( symbol_list, code_snippet_to_search, include_file_path )
	return_value = ""
	symbol_list.each do |symbol|
			if isSymbolFound(code_snippet_to_search,symbol)
				return "#include \"#{include_file_path}\"\n"
			end
		end
	return_value
end