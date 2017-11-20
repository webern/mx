
require 'fileutils'

###########################################################################
#
# Define directory paths
#
###########################################################################

name_mx_pkg = "mx-src-pkg"
name_mx_src = "Sourcecode"
name_mx_proj = "Xcode"

dir_this_ruby_script = File.dirname(File.realpath(__FILE__))
dir_mx_root = "#{dir_this_ruby_script}/.."
dir_mx_src = "#{dir_mx_root}/#{name_mx_src}"
dir_mx_proj = "#{dir_mx_root}/#{name_mx_proj}"
dir_komp_root = "/Users/mjb/komp"
dir_komp_mx_root = "#{dir_komp_root}/Frameworks/mx"
dir_komp_mx_deploy = "#{dir_komp_mx_root}/src-pkg"
dir_temp_root = "/Users/mjb/mx-temp"

file_mx_version_defines_h = "#{dir_mx_src}/mx/impl/MxVersionDefines.h"

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
# Copy sourcecode and Xcode projects to a temporary directory, then zip the
# the temporary directory and delete the unzipped version.
#
###########################################################################

dir_temp = "#{dir_temp_root}/#{name_mx_pkg}"
name_mx_pkg_zipped = "#{name_mx_pkg}.tar.gz"
file_zipped = "#{dir_temp_root}/#{name_mx_pkg_zipped}"
FileUtils.rm_rf(dir_temp)
FileUtils::mkdir_p dir_temp
FileUtils.cp_r "#{dir_mx_src}", "#{dir_temp}/#{name_mx_src}", :verbose => true
FileUtils.cp_r "#{dir_mx_proj}", "#{dir_temp}/#{name_mx_proj}", :verbose => true
system( "cd \"#{dir_temp_root}\" && tar -zcvf \"#{name_mx_pkg_zipped}\" \"#{name_mx_pkg}\"" )
FileUtils.rm_rf("#{dir_temp}")

###########################################################################
#
# Copy zipped sourcecode to the komp repo and create a file there which
# indicates the version which is in use
#
###########################################################################
FileUtils.cp "#{file_zipped}", "#{dir_komp_mx_deploy}"
file_current = "#{dir_komp_mx_deploy}/current.txt"
File.delete(file_current) if File.exist?(file_current)
open(file_current, 'w') { |f|
	f << "#{name_mx_pkg_zipped}"
}

###########################################################################
#
# Make a commit in the mx repo to record this version
#
###########################################################################
Dir.chdir dir_mx_root
system ( "git add --all" )
system ( "git commit -m\"deployed #{name_mx_pkg}\"" )

