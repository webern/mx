require 'fileutils'
require 'tmpdir'

###########################################################################
#
# Define directory and file paths
#
###########################################################################

configuration = "Release"

name_mx_pkg = "mx-version"
name_mx_src = "Sourcecode"
name_mx_proj = "Xcode"
name_versioning_file = "current.txt"
name_ios_target = "MxiOS"
name_osx_target = "MxmacOS"
name_ios = "lib#{name_ios_target}"
name_osx = "lib#{name_osx_target}"
name_ios_framework = "#{name_ios_target}.framework"
name_osx_framework = "#{name_osx_target}.framework"

dir_build = "/Users/mjb/mx-temp"
dir_this_ruby_script = File.dirname(File.realpath(__FILE__))
dir_mx_root = File.expand_path("#{dir_this_ruby_script}/..")
file_mx_xcode_workspace = "#{dir_mx_root}/Xcode/mx.xcworkspace"
file_mx_version_defines_h = "#{dir_mx_root}/Sourcecode/mx/impl/MxVersionDefines.h"

dir_komp_root = "/Users/mjb/repos/komp"
dir_komp_mx = "#{dir_komp_root}/Frameworks/mx"
dir_komp_mx_bin = "#{dir_komp_mx}/bin"
dir_komp_mx_src = "#{dir_komp_mx}/src"
dir_komp_mx_headers = "#{dir_komp_mx}/headers"


###########################################################################
#
# Find the current version, increment the build number by one, and save
# it. Use the resulting version to define the name of the package that we
# are deploying and store it to name_mx_pkg suffixing the value that was
# given above with the version number.
#
###########################################################################

version_major = 0
version_minor = 0
version_patch = 0
version_build = 0

f = File.open("#{file_mx_version_defines_h}", "r")
f.each_line do |line|
  if (line[0..24] <=> "#define MX_VERSION_MAJOR ") == 0
    version_major = Integer( line[25..-1] )
  end
  if (line[0..24] <=> "#define MX_VERSION_MINOR ") == 0
    version_minor = Integer( line[25..-1] )
  end
  if (line[0..24] <=> "#define MX_VERSION_PATCH ") == 0
    version_patch = Integer( line[25..-1] )
  end
  if (line[0..24] <=> "#define MX_VERSION_BUILD ") == 0
    version_build = Integer( line[25..-1] )
  end
end

version_build = Integer( version_build ) + 1
FileUtils.rm("#{file_mx_version_defines_h}")
open("#{file_mx_version_defines_h}", 'w') { |f|
  f << "#define MX_VERSION_MAJOR #{version_major}\n"
  f << "#define MX_VERSION_MINOR #{version_minor}\n"
  f << "#define MX_VERSION_PATCH #{version_patch}\n"
  f << "#define MX_VERSION_BUILD #{version_build}\n"
}

version = "#{version_major}.#{version_minor}.#{version_patch}.#{version_build}"
name_mx_pkg = "#{name_mx_pkg}-#{version}"
puts "deploying #{name_mx_pkg}"

###########################################################################
#
# Build Mx
#
###########################################################################

build_ios = "xcodebuild \
  -workspace #{file_mx_xcode_workspace} \
  -scheme #{name_ios_target} \
  build \
  -derivedDataPath #{dir_build} \
  -destination generic/platform=iOS \
  -destination 'generic/platform=iOS simulator' \
  -configuration #{configuration}"

is_success = system( build_ios )

if !is_success
  raise 'ios build failed'
end

build_mac = "xcodebuild \
  -workspace #{file_mx_xcode_workspace} \
  -scheme #{name_osx_target} \
  build \
  -derivedDataPath #{dir_build} \
  -destination generic/platform=macOS \
  -destination 'platform=OS X,arch=x86_64' \
  -configuration #{configuration}"

is_success = system( build_mac )

if !is_success
  raise 'macos build failed'
end

###########################################################################
#
# Create a directory for the output
#
###########################################################################

dir_out = "#{dir_build}/../mx-out"
FileUtils.rm_rf(dir_out)
FileUtils::mkdir_p dir_out

###########################################################################
#
# Move and Lipo the frameworks
#
###########################################################################

dir_ios_framework_fat = "#{dir_out}/#{name_ios_framework}"
dir_ios_iphone_only = "#{dir_build}/Build/Products/#{configuration}-iphoneos/#{name_ios_framework}"
dir_ios_i386___only = "#{dir_build}/Build/Products/#{configuration}-iphonesimulator/#{name_ios_framework}"
FileUtils.rm_rf(dir_ios_framework_fat)
FileUtils.mkdir_p(dir_ios_framework_fat)
FileUtils::copy_entry(dir_ios_iphone_only, dir_ios_framework_fat)

file_lipo_output = "#{dir_out}/#{name_ios_framework}/#{name_ios_target}"
file_lipo_input_a = "#{dir_ios_iphone_only}/#{name_ios_target}"
file_lipo_input_b = "#{dir_ios_i386___only}/#{name_ios_target}"

FileUtils.rm(file_lipo_output)

cmd_lipo = "lipo -create -output \"#{file_lipo_output}\" \"#{file_lipo_input_a}\" \"#{file_lipo_input_b}\""
is_success = system( cmd_lipo )

if !is_success
  raise "lipo failed"
end

dir_osx_framework = "#{dir_out}/#{name_osx_framework}"
dir_osx_build_result = "#{dir_build}/Build/Products/#{configuration}/#{name_osx_framework}"
FileUtils.rm_rf(dir_osx_framework)
FileUtils.mkdir_p(dir_osx_framework)
FileUtils::copy_entry(dir_osx_build_result, dir_osx_framework)

###########################################################################
#
# Deploy to Komp
#
###########################################################################

dir_komp_mx_ios_framework = "#{dir_komp_mx_bin}/#{name_ios_framework}"
FileUtils.rm_rf(dir_komp_mx_ios_framework)
FileUtils.mkdir_p(dir_komp_mx_ios_framework)
FileUtils::copy_entry(dir_ios_framework_fat, dir_komp_mx_ios_framework)

dir_komp_mx_osx_framework = "#{dir_komp_mx_bin}/#{name_osx_framework}"
FileUtils.rm_rf(dir_komp_mx_osx_framework)
FileUtils.mkdir_p(dir_komp_mx_osx_framework)
FileUtils::copy_entry(dir_osx_build_result, dir_komp_mx_osx_framework)

###########################################################################
#
# Copy Headers
#
###########################################################################

FileUtils.rm_rf(dir_komp_mx_headers)
FileUtils::mkdir_p "#{dir_komp_mx_headers}/mx/api"

file_header_list = Dir["#{dir_mx_root}/Sourcecode/mx/api/*.h"]

file_header_list.each do |file_current_header|
  FileUtils.cp(file_current_header, "#{dir_komp_mx_headers}/mx/api")
end

###########################################################################
#
# Write version info
#
###########################################################################

name_version = "current-mx-version.txt"
file_version_bin = "#{dir_komp_mx_bin}/#{name_version}"
file_version_src = "#{dir_komp_mx_src}/#{name_version}"
file_version_hed = "#{dir_komp_mx_headers}/#{name_version}"

File.delete(file_version_bin) if File.exist?(file_version_bin)
open(file_version_bin, 'w') { |f|
  f << "#{name_mx_pkg}"
}

File.delete(file_version_src) if File.exist?(file_version_src)
open(file_version_src, 'w') { |f|
  f << "#{name_mx_pkg}"
}

File.delete(file_version_hed) if File.exist?(file_version_hed)
open(file_version_hed, 'w') { |f|
  f << "#{name_mx_pkg}"
}

###########################################################################
#
# zip and save a copy of mx sourcecode to the komp repo for backup
#
###########################################################################

dir_code_copy_before_zipping = "#{dir_build}/#{name_mx_pkg}"
name_zipped_filename = "#{name_mx_pkg}.tar.gz"
file_zipped_source = "#{dir_build}/#{name_zipped_filename}"
FileUtils.mkdir_p "#{dir_code_copy_before_zipping}/Sourcecode"
FileUtils.cp_r "#{dir_mx_root}/Sourcecode", "#{dir_code_copy_before_zipping}/Sourcecode"
FileUtils.mkdir_p "#{dir_code_copy_before_zipping}/Xcode"
FileUtils.cp_r "#{dir_mx_root}/Xcode", "#{dir_code_copy_before_zipping}/Xcode"
system( "cd \"#{dir_build}\" && tar -zcvf \"#{name_zipped_filename}\" \"#{name_mx_pkg}\"" )
FileUtils.cp "#{file_zipped_source}", "#{dir_komp_mx_src}"
FileUtils.rm_rf(dir_code_copy_before_zipping)

###########################################################################
#
# make a commit in the git repo that corresponds to this deployment
#
###########################################################################

Dir.chdir dir_mx_root
system ( "git add --all" )
system ( "git commit -m\"#{name_mx_pkg}\"" )