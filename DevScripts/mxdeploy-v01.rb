
require 'fileutils'

configuration = "Release"
config = configuration.downcase

build_root = "/Users/mjb/mxout"
mx_repo_root = "/Users/mjb/Dropbox/Programming/MxRepo"
komp_repo_root = "Users/mjb/komp"
lib_ios = "libMx-iOS.a"
lib_macOS = "libMx-macOS.a"
mx_version_defines_h = "#{mx_repo_root}/Sourcecode/mx/impl/MxVersionDefines.h"
upload_dir = "/Users/mjb/Dropbox/Programming/MxKompDeploy"

version_major = 0
version_minor = 0
version_patch = 0
version_build = 0

f = File.open("#{mx_version_defines_h}", "r")
f.each_line do |line|
  if (line[0..24] <=> "#define MX_VERSION_MAJOR ") == 0
  	version_major = Integer( line[25..-1] )
  end
end

f.close
f = File.open("#{mx_version_defines_h}", "r")
f.each_line do |line|
  if (line[0..24] <=> "#define MX_VERSION_MINOR ") == 0
  	version_minor = Integer( line[25..-1] )
  end
end

f.close
f = File.open("#{mx_version_defines_h}", "r")
f.each_line do |line|
  if (line[0..24] <=> "#define MX_VERSION_PATCH ") == 0
  	version_patch = Integer( line[25..-1] )
  end
end
f.close

f = File.open("#{mx_version_defines_h}", "r")
f.each_line do |line|
  if (line[0..24] <=> "#define MX_VERSION_BUILD ") == 0
  	version_build = Integer( line[25..-1] )
  end
end
f.close

version_build = Integer( version_build ) + 1

FileUtils.rm("#{mx_version_defines_h}")

open("#{mx_version_defines_h}", 'w') { |f|
	f << "#define MX_VERSION_MAJOR #{version_major}\n"
	f << "#define MX_VERSION_MINOR #{version_minor}\n"
	f << "#define MX_VERSION_PATCH #{version_patch}\n"
	f << "#define MX_VERSION_BUILD #{version_build}\n"
}

version = "#{version_major}.#{version_minor}.#{version_patch}.#{version_build}"
puts "building version #{version}"

build_ios = "xcodebuild \
	-workspace #{mx_repo_root}/Xcode/mx.xcworkspace \
	-scheme Mx-iOS \
	build \
	-derivedDataPath #{build_root} \
	-destination generic/platform=iOS \
	-destination 'generic/platform=iOS simulator' \
	-configuration #{configuration}"

is_success = system( build_ios )

if !is_success
	raise 'ios build failed'
end

build_mac = "xcodebuild \
	-workspace #{mx_repo_root}/Xcode/mx.xcworkspace \
	-scheme Mx-macOS \
	build \
	-derivedDataPath #{build_root} \
	-destination generic/platform=macOS \
	-destination 'platform=OS X,arch=x86_64' \
	-configuration #{configuration}"

is_success = system( build_mac )

if !is_success
	raise 'macos build failed'
end

universal_output_dir = "#{build_root}/Build/Universal/#{configuration}"
FileUtils.rm_rf(universal_output_dir)
FileUtils::mkdir_p universal_output_dir

lipo_output_a = "#{universal_output_dir}/#{lib_ios}"
ios_input_a = "#{build_root}/Build/Products/#{configuration}-iphoneos/#{lib_ios}"
i386_input_a = "#{build_root}/Build/Products/#{configuration}-iphonesimulator/#{lib_ios}"
x86_64_a = "#{build_root}/Build/Products/#{configuration}/#{lib_macOS}"

lipo_cmd = "lipo -create -output \"#{lipo_output_a}\" \"#{ios_input_a}\" \"#{i386_input_a}\""
is_success = system( lipo_cmd )

if !is_success
	raise 'lipo failed'
end

final_name = "mx-deploy-#{config}-#{version}"
final_output_dir = "#{build_root}/#{final_name}"
FileUtils.rm_rf(final_output_dir)
FileUtils::mkdir_p final_output_dir

FileUtils.cp(lipo_output_a, "#{final_output_dir}")
FileUtils.cp(x86_64_a, "#{final_output_dir}")

header_output_dir = "#{final_output_dir}/include/mx/api"
FileUtils::mkdir_p header_output_dir
FileUtils.copy_entry "#{mx_repo_root}/Sourcecode/mx/api", "#{header_output_dir}"

system( "cd \"#{build_root}\" && tar -zcvf \"#{final_name}.tar.gz\" \"#{final_name}\"" )

FileUtils.cp("#{build_root}/#{final_name}.tar.gz", "#{upload_dir}")
FileUtils.rm_rf(final_output_dir)
FileUtils.rm_rf("#{build_root}/#{final_name}.tar.gz")
