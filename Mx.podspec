Pod::Spec.new do |spec|
  spec.name         = 'Mx'
  spec.version      = '99.99.0'
  spec.license      = { :type => 'MIT' }
  spec.homepage     = 'https://github.com/webern/musicxml-class-library'
  spec.authors      = { 'Matthew James Briggs' => 'matthew.james.briggs@gmail.com' }
  spec.summary      = 'A C++ Library for MusicXML'
  spec.source       = { :git => 'https://github.com/webern/musicxml-class-library.git', :tag => 'v99.99.0' }
  spec.source_files = 'Sourcecode/mx/**/*.{h,cpp}'
  spec.header_dir = 'Sourcecode'
  spec.public_header_files = "Sourcecode/mx/**/*.h"
  spec.header_mappings_dir = 'Sourcecode'
  #spec.preserve_paths = "Sourcecode/*", "Sourcecode/**/*"
  spec.ios.deployment_target  = '10.0'
  spec.osx.deployment_target  = '10.7'
end