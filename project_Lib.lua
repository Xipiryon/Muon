
-- Library
-------------------------------------------

project "Muon"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetdir (SolutionRoot.."/bin/lib")

	files {
       ProjectRoot.."/src/**.cpp",
       ProjectRoot.."/include/**.hpp",
    }

	filter "Debug*"
		defines { "MUON_DEBUG"}

	filter  "*DLL"
		defines { "MUON_EXPORTS" }

