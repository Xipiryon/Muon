
-- Unit Tests
-------------------------------------------

project "UnitTests"
	language "C++"
	targetname "UnitTests"
	targetdir (SolutionRoot.."/bin")
	kind "ConsoleApp"

	files	{
		ProjectRoot.."/unittests/main.cpp"
	}

	filter "Debug*"
		links	{ "Muon-d" }
	filter "Release*"
		links { "Muon" }
