
-- Library
-------------------------------------------

project "Muon"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "Muon"
	targetdir (SolutionRoot.."/bin/lib")

	if os.is("windows") then
		postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/Muon*.dll "..SolutionRoot.."/bin/", "/", "\\") }
	else
		postbuildcommands { "find "..SolutionRoot.."/bin/lib/ -name libMuon*.so -exec cp {} "..SolutionRoot.."/bin/ \\;" }
	end

	files {
       ProjectRoot.."/src/**.cpp",
       ProjectRoot.."/include/**.hpp",
    }

	filter  "*DLL"
		defines { "MUON_EXPORTS" }

