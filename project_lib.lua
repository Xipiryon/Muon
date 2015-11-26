
-- Library
-------------------------------------------

project "Muon"

	language "C++"
	targetdir (SolutionRoot.."/bin")

	files {
       ProjectRoot.."/src/**.cpp",
       ProjectRoot.."/include/**.hpp",
    }

	filter  "*DLL"
		defines { "MUON_EXPORTS" }

