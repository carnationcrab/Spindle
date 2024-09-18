workspace "Spindle"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Spindle"
    location "Spindle"
    kind "SharedLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")


    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion  "latest"

        defines
        {
            "SPINDLE_PLATFORM_WINDOWS",
            "SPINDLE_BUILD_DLL"
        }

        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"
        }

    -- optimization goes on for real time testing but logging is on for debug
    filter "configurations:Debug"
        defines "SPINDLE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "SPINDLE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "SPINDLE_DIST"
        optimize "On"

    -- use multithreaded library
    filter { "system:windows", "configurations:Release"}
        buildoptions "/MT"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")


    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Spindle/vendor/spdlog/include",
        "Spindle/src"
    }

    links
    {
        "Spindle"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SPINDLE_PLATFORM_WINDOWS"
        }
    
    -- optimization goes on for real time testing but logging is on for debug

    filter "configurations:Debug"
        defines "SPINDLE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "SPINDLE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "SPINDLE_DIST"
        optimize "On"

    -- use multithreaded library
    filter { "system:windows", "configurations:Release"}
        buildoptions "/MT"
