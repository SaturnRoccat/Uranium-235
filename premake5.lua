workspace "Uranium"
    configurations { "Debug", "Release" }
    architecture "x64"
    location "solutions"

    

project "Uranium_235"
    architecture "x86_64"
    kind "StaticLib"
    language "C++"
    targetdir "out/bin/Uranium_235/%{cfg.buildcfg}"

    -- include the include directory
    includedirs { "include/"}

    -- Use C++ 20
    cppdialect "C++20"
    files { "UraniumAPI/**.h", "UraniumAPI/**.cpp", "UraniumAPI/**.hpp" }

    -- Copy all headers into the build directory following the same directory structure and in a folder called "include/Uranium_235"
    postbuildcommands {
        'python "../clone.py" ../UraniumAPI/ ../out/bin/Uranium_235/%{cfg.buildcfg}/include/Uranium_235/'
    }

    platforms { "x86_64" } -- Screw 32 bit

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
        symbols "On"

    vpaths {
        ["UraniumAPI/*"] = { "UraniumAPI/**.h", "UraniumAPI/**.cpp", "UraniumAPI/**.hpp" }
    }

project "Uranium-234"
    architecture "x86_64"
    kind "ConsoleApp"
    language "C++"
    targetdir "out/bin/Uranium-234/%{cfg.buildcfg}"
    cppdialect "C++20"
    files { "Uranium-234/**.h", "Uranium-234/**.cpp", "Uranium-234/**.hpp" }
    includedirs { "include/" } 
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        debugdir "out/bin/Uranium-234/%{cfg.buildcfg}"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
        debugdir "out/bin/Uranium-234/%{cfg.buildcfg}"
        symbols "On"

    vpaths {
        ["Uranium-234/*"] = { "Uranium-234/**.h", "Uranium-234/**.cpp", "Uranium-234/**.hpp" }
    }

project "Uranium_235_tester"
    architecture "x86_64"
    kind "ConsoleApp"
    language "C++"
    targetdir "out/bin/Uranium_235_tester/%{cfg.buildcfg}"

    -- Use C++ 20
    cppdialect "C++20"

    files { "UraniumTester/**.h", "UraniumTester/**.cpp", "UraniumTester/**.hpp" }

    includedirs { "UraniumAPI", "include/" }
    links { "Uranium_235" }
    platforms { "x86_64" } -- Screw 32 bit

    filter "configurations:Debug"
        defines { "DEBUG", "BYPASSCHECKS" }
        debugdir "out/bin/Uranium-234/%{cfg.buildcfg}"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
        debugdir "out/bin/Uranium-234/%{cfg.buildcfg}"
        symbols "On"

    filter "configurations:UraniumReleaseDev"
        defines { "RELEASE", "BYPASSCHECKS" }
        optimize "On"
        debugdir "out/bin/Uranium-234/%{cfg.buildcfg}"
        symbols "On"

    vpaths {
        ["UraniumTester/*"] = { "UraniumTester/**.h", "UraniumTester/**.cpp", "UraniumTester/**.hpp" }
    }
