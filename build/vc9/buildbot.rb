# Build Bot
# Version 1.2
# ====================================

require 'YAML'
require 'rubygems'
require 'webster'
require 'fileutils'
require 'digest/md5'

PREFS = YAML::load(DATA)

if (File.exists?('../.developer') == false)
	puts "This is not a developer SDK. I will not run."
	Process.exit
end

def escape_if(cond, reason = nil)
	if (cond)
		puts "[BuildBot] #{reason}" if reason != nil
		Process.exit
	end
end

def comment(line)
	puts "[BuildBot] #{line}"
end

def do_version(args)
	escape_if PREFS["version_file"] == nil, "Version file not given in configuration"
	f = File.read(PREFS["version_file"])
	escape_if f == nil, "Could not find or read file"
	prefix = (PREFS["project"] || "Project")

	force = false
	if args[0] != nil
		force = true if args[0].to_s.downcase == 'force'	
	end

	hsh = nil
	mhsh = nil
	# Try and fetch hash
	res = f.scan /\/\/~~\s+mtime\s+(\w+)/
	if res[0] != nil
		hsh = res[0][0].downcase.strip if res[0][0] != nil
	end

	if force == false
		mdates = Array.new
		Dir.glob(PREFS["cpp_files"]).each do |fn|
			next if (File.basename(fn) == File.basename(PREFS["unity_build_file"]) )
			mdates.push File.stat(fn).mtime.to_i.to_s
		end
		Dir.glob(PREFS["h_files"]).each do |fn|
			next if (File.basename(fn) == File.basename(PREFS["version_file"]) )
			mdates.push File.stat(fn).mtime.to_i.to_s
		end
		mhsh = Digest::MD5.hexdigest(mdates.join(',')).downcase

		return if hsh == mhsh
		
		###########

		# Bump build number.
		f.gsub! /^\s*#define\s+#{prefix}VersionBuild\s+\d+/ do |ln|
			build = ln.scan(/Build\s+(\d+)/)[0][0]
			ln.gsub(build.to_s, (build.to_i + 1).to_s)
		end

		# Bump modified time
		f.gsub! /^\s*\/\/~~\s+mtime\s+\w+/ do |ln|
			ln.gsub(/mtime\s+\w+/, "mtime " + mhsh)
		end

	end

	# Bump build name
	webster = Webster.new
	build_name = webster.random_word.capitalize

	f.gsub! /^\s*#define\s+CritterBuildName\s+"\w+"/ do |ln|
		ln.gsub(/\"\w+\"/, "\"#{build_name}\"")
	end

	File.open(PREFS["version_file"], 'w')  { |c| c.write f }
	
	comment "This is build '#{build_name}'"
end

def do_unitybuild(args)
	escape_if PREFS["cpp_files"] == nil, "No .cpp files given in the configuration"
	escape_if PREFS["unity_build_file"] == nil, "No Unity Build file given in the configuration"

	f = File.read(PREFS["unity_build_file"])

	refreshRequired = false
	hsh = nil
	mhsh = nil
	# Try and fetch hash
	res = f.scan /\/\/~~\s+ub_begin\s+(\w+)/
	if res[0] != nil
		hsh = res[0][0].downcase.strip if res[0][0] != nil
	end

	# Compare hash with generated hash of the cpp file modified dates
	# If there is no hash, then a refresh is required anyway, as the file is new.
	if hsh != nil
		mfiles = Array.new
		Dir.glob(PREFS["cpp_files"]).each do |fn|
			mfiles.push File.basename(fn)
		end
		mhsh = Digest::MD5.hexdigest(mfiles.join(',')).downcase
		refreshRequired = true if mhsh != hsh
	else
		refreshRequired = true
	end
	
	if refreshRequired
		
		escape_if((f.scan(/\/\/~~\s+ub_begin/)[0] == nil), "Cannot find '~~ ub_begin comment' in #{PREFS["unity_build_file"]}")
		escape_if((f.scan(/\/\/~~\s+ub_end/)[0] == nil), "Cannot find '~~ ub_end comment' #{PREFS["unity_build_file"]}")
		
		if (mhsh == nil)
			mdates = Array.new
			Dir.glob(PREFS["cpp_files"]).each do |fn|
				mfiles.push File.basename(fn)
			end
			mhsh = Digest::MD5.hexdigest(mdates.join(',')).downcase
		end

		cpp = String.new
		cpp << "//~~ ub_begin #{mhsh}\n\n"

		Dir.glob(PREFS["cpp_files"]).each do |fn|
			next if (File.basename(fn) == File.basename(PREFS["unity_build_file"]))
			cpp << "#include \"#{File.basename(fn)}\"\n"
		end
		cpp << "\n//~~ ub_end\n"
		f.gsub! /(^\s*\/\/~~\s+ub_begin.*^\s*\/\/~~\s+ub_end)\n/m, cpp
		
		File.open(PREFS["unity_build_file"], 'w')  { |c| c.write f }

		comment "Changes made to source files. Generating new UnityBuild file."
	end
end

def do_mvscsln(args)

	path = args[0]
	args.delete_at 0

	path << "/" if path[-1] != "/"

	puts path

	file_suffix = { 2008 => "vc9", 2005 => "vc8", 2003 => "vc71", 2002 => "vc70" }
	sln_version_full = { 2008 => "Visual Studio 2008", 2005 => "Visual Studio 2005", 2003 => "Visual Studio 2003", 2002 => "Visual Studio 2002" }
	sln_version = { 2008 => "10.0", 2005 => "9.0", 2003 => "8.0", 2002 => "7.0" }
	vcproj_version = { 2008 => "9.00", 2005 => "8.00", 2003 => "7.10", 2002 => "7.00"}

	args.each do |vers|
		vers = vers.to_i

		sln = String.new
		sln << PREFS["msvc_sln"]

		sln.gsub!('$sln_version$', sln_version[vers])
		sln.gsub!('$sln_version_full$', sln_version_full[vers])
		sln.gsub!('$target$', file_suffix[vers] + "/" "Critter" +  ".vcproj" )

		File.open(path + "Critter_" + file_suffix[vers] + ".sln", 'w')  { |c| c.write sln }

		vcproj = String.new
		vcproj << PREFS["msvc_vcproj"]
		vcproj.gsub!('$vcproj_version$', vcproj_version[vers])
		
		Dir.mkdir(path + file_suffix[vers]) if !File::directory?(path + file_suffix[vers])
		File.open(path + file_suffix[vers] + "/Critter.vcproj", 'w')  { |c| c.write vcproj }

		FileUtils.cp('buildbot.bat', path + file_suffix[vers])
		FileUtils.cp('buildbot.rb', path + file_suffix[vers])
		
	end
end

escape_if ARGV.size == 0, "Wrong number of arguments given"

string_args = ARGV.join(',')
main_args = string_args.split('and')

main_args.each do |a|
	args = a.split(',')
	args.delete(String.new)
	tool = args[0].downcase
	args.delete_at(0)

	do_version(args) if tool == 'version'
	do_unitybuild(args) if tool == 'unitybuild'
	do_mvscsln(args) if tool == "msvcsln"
end

__END__

# Configuration (as YAML)
version_file: ../source/CritterVersion.h
cpp_files:
  - ../source/Critter*.cpp
h_files:
  - ../source/*.h
unity_build_file: ../source/Critter.cpp
project: Critter
msvc_sln: |
  Microsoft Visual Studio Solution File, Format Version $sln_version$
  # $sln_version_full$
  Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Critter", "$target$", "{45C50569-9FEB-E8AC-CA69-E9664FAEB81E}"
  EndProject
  Global
  GlobalSection(SolutionConfigurationPlatforms) = preSolution
  DebugStatic|Win32 = DebugStatic|Win32
  FinalStatic|Win32 = FinalStatic|Win32
  EndGlobalSection
  GlobalSection(ProjectConfigurationPlatforms) = postSolution
  {45C50569-9FEB-E8AC-CA69-E9664FAEB81E}.DebugStatic|Win32.ActiveCfg = DebugStatic|Win32
  {45C50569-9FEB-E8AC-CA69-E9664FAEB81E}.DebugStatic|Win32.Build.0 = DebugStatic|Win32
  {45C50569-9FEB-E8AC-CA69-E9664FAEB81E}.FinalStatic|Win32.ActiveCfg = FinalStatic|Win32
  {45C50569-9FEB-E8AC-CA69-E9664FAEB81E}.FinalStatic|Win32.Build.0 = FinalStatic|Win32
  EndGlobalSection
  GlobalSection(SolutionProperties) = preSolution
  HideSolutionNode = FALSE
  EndGlobalSection
  EndGlobal
msvc_vcproj: |
  <?xml version="1.0" encoding="Windows-1252"?>
  <VisualStudioProject ProjectType="Visual C++" Version="$vcproj_version$" Name="Critter" ProjectGUID="{45c50569-9feb-e8ac-ca69-e9664faeb81e}" Keyword="Win32Proj" TargetFrameworkVersion="131072" >
      <Platforms>
            <Platform Name="Win32" />
      </Platforms>
      <ToolFiles> </ToolFiles>
      <Configurations>
            <Configuration Name="FinalStatic|Win32" OutputDirectory="$(ProjectDir)\FinalStatic" IntermediateDirectory="$(ProjectDir)\FinalStatic" ConfigurationType="4" CharacterSet="1" >
                  <Tool Name="VCPreBuildEventTool" CommandLine="" />
                  <Tool Name="VCCustomBuildTool" />
                  <Tool Name="VCXMLDataGeneratorTool" />
                  <Tool Name="VCWebServiceProxyGeneratorTool" />
                  <Tool Name="VCMIDLTool" />
                  <Tool Name="VCCLCompilerTool" AdditionalIncludeDirectories="..\source;$(NXOGRE_DIR)\SDK;$(OGRE_HOME)\include;$(OGRE_HOME)\include\OGRE;$(OGRE_HOME)\boost_1_42" PreprocessorDefinitions="WIN32;_LIB;CRITTER_SDK;CRITTER_STATIC_LIBRARY" RuntimeLibrary="2" />
                  <Tool Name="VCManagedResourceCompilerTool" />
                  <Tool Name="VCResourceCompilerTool" />
                  <Tool Name="VCPreLinkEventTool" />
                  <Tool Name="VCLibrarianTool" AdditionalDependencies="OgreMain.lib NxOgre.lib" OutputFile="$(OutDir)\CritterStatic.lib" AdditionalLibraryDirectories="&quot;$(OGRE_HOME)\lib\release&quot;;&quot;$(NXOGRE_DIR)\sdk\&quot;" />
                  <Tool Name="VCALinkTool" />
                  <Tool Name="VCXDCMakeTool" />
                  <Tool Name="VCBscMakeTool" />
                  <Tool Name="VCFxCopTool" />
                  <Tool Name="VCPostBuildEventTool" CommandLine="buildbot.bat makesdk FinalStatic" />
            </Configuration>
            <Configuration Name="DebugStatic|Win32" OutputDirectory="$(ProjectDir)\DebugStatic" IntermediateDirectory="$(ProjectDir)\DebugStatic" ConfigurationType="4" CharacterSet="1" >
                  <Tool Name="VCPreBuildEventTool" CommandLine="" />
                  <Tool Name="VCCustomBuildTool" />
                  <Tool Name="VCXMLDataGeneratorTool" />
                  <Tool Name="VCWebServiceProxyGeneratorTool" />
                  <Tool Name="VCMIDLTool" />
                  <Tool Name="VCCLCompilerTool" AdditionalOptions="/Zm140" AdditionalIncludeDirectories="..\source;$(NXOGRE_DIR)\SDK;$(OGRE_HOME)\include;$(OGRE_HOME)\include\OGRE;$(OGRE_HOME)\boost_1_42" PreprocessorDefinitions="WIN32;_LIB;CRITTER_SDK;CRITTER_STATIC_LIBRARY" MinimalRebuild="true" RuntimeLibrary="3" WarningLevel="2" DebugInformationFormat="4" />
                  <Tool Name="VCManagedResourceCompilerTool" />
                  <Tool Name="VCResourceCompilerTool" />
                  <Tool Name="VCPreLinkEventTool" />
                  <Tool Name="VCLibrarianTool" AdditionalDependencies="OgreMain_d.lib NxOgreDebug.lib" OutputFile="$(OutDir)\CritterDebugStatic.lib" AdditionalLibraryDirectories="&quot;$(OGRE_HOME)\lib\debug&quot;;&quot;$(NXOGRE_DIR)\sdk\&quot;" />
                  <Tool Name="VCALinkTool" />
                  <Tool Name="VCXDCMakeTool" />
                  <Tool Name="VCBscMakeTool" />
                  <Tool Name="VCFxCopTool" />
                  <Tool Name="VCPostBuildEventTool" CommandLine="buildbot.bat makesdk DebugStatic" />
            </Configuration>
      </Configurations>
      <References> </References>
      <Files>
            <File RelativePath="..\source\Critter.h" > </File>
            <File RelativePath="..\source\Critter.cpp" > </File>
      </Files>
      <Globals> </Globals>
  </VisualStudioProject>