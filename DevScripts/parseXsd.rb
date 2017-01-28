require 'rexml/document'
require 'rexml/xpath'
doc = REXML::Document.new(File.new('../Documents/musicxml.xsd'))

stuff = doc.elements.to_a

REXML::XPath.each(doc, "//xs:element") { |element|
	puts element.attributes["name"]
}