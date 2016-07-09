
def isSymbolFound ( code_string_to_search_in, symbol_name_to_find )
	rx = /(\s#{symbol_name_to_find}\s|\s#{symbol_name_to_find}\&)/
	( code_string_to_search_in =~ rx ) != nil
end