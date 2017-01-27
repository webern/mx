Pod::Spec.new do |spec|
  spec.name         = 'Mx'
  spec.version      = '999.999.0'
  spec.license      = { :type => 'MIT' }
  spec.homepage     = 'https://github.com/webern/musicxml-class-library'
  spec.authors      = { 'Matthew James Briggs' => 'matthew.james.briggs@gmail.com' }
  spec.summary      = 'A C++ Library for MusicXML'
  spec.source       = { :git => 'https://github.com/webern/musicxml-class-library.git', :tag => '99.99.0' }
  spec.source_files = 'Sourcecode/mx/**/*.{h,cpp}'
end