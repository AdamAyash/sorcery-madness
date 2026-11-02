project "GameClient"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
externalwarnings("Off")

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files {
    "source/**.h",
    "source/**.cpp",
}

links {
    "Networking",
    "Engine"
}

includedirs {
    IncludeDirectories["Common"],
    IncludeDirectories["Networking"],
    IncludeDirectories["spdlog"],
    IncludeDirectories["Engine"],
    IncludeDirectories["GLFW"],
    IncludeDirectories["Glad"]
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"
