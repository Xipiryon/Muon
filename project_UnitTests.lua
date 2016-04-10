
-- Unit Tests
-------------------------------------------

project "Muon_UnitTests"
	local ProjectRoot = os.getcwd()

	dependson("Muon_Core")
	language "C++"
	targetname "UnitTests"
	targetdir (SolutionRoot.."/bin")
	kind "ConsoleApp"

	files	{
		ProjectRoot.."/unittests/main.cpp",
		ProjectRoot.."/unittests/tinyxml2.cpp",
		ProjectRoot.."/unittests/tinyxml2.h"
	}

	filter "Debug*"
		links	{ "Muon-d" }

	filter "Release*"
		links { "Muon-r" }

	filter "Final*"
		links { "Muon-f" }

	filter {}
