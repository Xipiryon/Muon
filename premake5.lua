------------------------------
-- Global Variable
------------------------------

-- Prefix Install specify where it should look for custom
-- project header & library files
if G_Install == nil then

	G_Install = {}
	-- Default Value on Windows & Unix Platform
	if os.is("windows") then
		G_Install.Root = "C:/Premake/"
	else
		G_Install.Root = "/usr/local/"
	end
end
if not G_Install.Header then G_Install.Header = G_Install.Root.."include" end
if not G_Install.Lib then G_Install.Lib = G_Install.Root.."lib" end

ProjectRoot = os.getcwd()

if SolutionRoot == nil then
	SolutionRoot = ProjectRoot
end
------------------------------
-- Solution
------------------------------

solution "Muon"

	if _OPTIONS["unittests"] then
		startproject "UnitTests"
	end
	configurations { "DebugDLL", "DebugLib", "ReleaseLib", "ReleaseDLL" }

	if os.is("windows") then
		implibdir "bin"
	else
		buildoptions { "--std=c++11" }
		linkoptions { "-Wl,-rpath,bin" }
	end

	-- If option exists, then override G_Install
	if _OPTIONS["basedir"] then
		G_Install.Root = _OPTIONS["basedir"]
		G_Install.Header = _OPTIONS["basedir"].."/include"
		G_Install.Lib = _OPTIONS["basedir"].."/lib"
		print("Install directory has been overwritten to '"..G_Install.Root.."'")
	end

	includedirs {
		ProjectRoot.."include",
		G_Install.Header,
	}

	libdirs {
		ProjectRoot.."bin",
		G_Install.Lib
	}

	filter "Debug*"
		targetsuffix "-d"
        flags   { "Symbols" }
		defines { "MUON_DEBUG"}

	filter "Release*"
		optimize "Speed"
		flags   { "LinkTimeOptimization", "NoRTTI" }
		defines { "MUON_RELEASE" }

    filter  "*Lib"
        kind "StaticLib"

    filter  "*DLL"
        kind "SharedLib"

------------------------------
-- Project
------------------------------

include("project_Lib")

if _OPTIONS["unittests"] then
	include("project_UnitTests")
end

------------------------------
-- Options
------------------------------

newoption {
	trigger     = "basedir",
	value       = "PATH",
	description = "Folder to search lib & include; default: '"..G_Install.Root.."'",
}

newoption {
	trigger     = "unittests",
	description = "Enable compilation of unit tests",
}

------------------------------
-- Actions
------------------------------

newaction {
	trigger     = "install",
	description = "Install developpment files & library",
	execute = function ()
		print("** Installing Header files in: "..G_Install.Header.." **")

		local incDir = ProjectRoot.."/include/"
		local libDir = ProjectRoot.."/bin/"

		-- HEADER
		-- Create required folders
		local dirList = os.matchdirs(incDir.."**")
		for _,fdir in pairs(dirList) do
			local destDir = G_Install.Header..string.sub(fdir, #incDir)

			if(not os.isdir(destDir)) then
				if os.mkdir(destDir) then print("Creating "..destDir) end
			end
		end

		-- Copy files
		local fileList = os.matchfiles(incDir.."**")
		for _,fpath in pairs(fileList) do
			local destFile = G_Install.Header..string.sub(fpath, #incDir)
			if os.copyfile(fpath, destFile) then print("Installing "..destFile) end
		end


		-- LIBRARY
		print("** Installing Library files in: "..G_Install.Lib.." **")
		local destDir = G_Install.Lib

		-- Create required folders
		if(not os.isdir(destDir)) then
			if os.mkdir(destDir) then print("Creating "..destDir) end
		end

		local exts = {}
		if os.is("windows") then
			exts[0] = ".dll"
			exts[1] = ".lib"
		else
			exts[0] = ".so"
			exts[1] = ".a"
		end

		-- Copy files
		for _,ext in pairs(exts) do
			local fileList = os.matchfiles(libDir.."**"..ext)
			for _,fpath in pairs(fileList) do
				local destFile = G_Install.Lib..string.sub(fpath, #libDir)
				if os.copyfile(fpath, destFile) then print("Installing "..destFile) end
			end
		end
	end
}
