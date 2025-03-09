workspace "Write"
    configurations {"Debug", "Release" }
    system "Windows"
    architecture "x64"

    -- Ensure the build directories exist
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/obj/%{cfg.buildcfg}"
    

project "imgui"
    kind "StaticLib"
    language "C++"
    files {
        "deps/imgui/*.cpp",
        "deps/imgui/*.h"
    }

    includedirs{
	"deps/sdl/include"
    }
    targetdir "bin/%{cfg.buildcfg}/imgui"
    location "build/imgui"

project "wapp"
    kind "ConsoleApp"
    language "C++"
    files {
        "wapp/src/**.cpp",
        "wapp/include/**.h"
    }

    includedirs {
	"deps/imgui/",
	"deps/sdl/include",
	"wapp/include/"
    }

    -- filter "system:Linux"
    -- 	print("To do linux")
    -- 	os.exit()
    
    filter "system:Windows"
        print("On Windows")
        libdirs {
            "deps/sdl/lib/x64",
	    "bin/%{cfg.buildcfg}/imgui"
        }
        links{
            "SDL3.lib",
	    "imgui.lib",
	    "opengl32"
        }
	-- postbuildcommands{
	--     "cp ./deps/sdl/lib/x64/SDL3.dll ./bin/%{cfg.buildcfg}/wapp"
	--    	}

    targetdir "bin/%{cfg.buildcfg}/wapp"
    location "build/wapp"
    
