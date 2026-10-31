project "Engine"
kind "StaticLib"
language "C++"
cppdialect "C++20"
externalwarnings("Off")

files {
    "source/**.h",
    "source/**.cpp",
}

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

filter "system:windows"
links {
    "Common",
    "opengl32",
    "GLFW",
}

includedirs {
    "%{wks.location}/Common/source",
    IncludeDirectories["spdlog"],
    IncludeDirectories["GLFW"],
    IncludeDirectories["Glad"]
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"

filter {}
