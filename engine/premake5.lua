project "Engine"
kind "StaticLib"
language "C++"
cppdialect "C++20"
externalwarnings("Off")

files {
    "source/**.h",
    "source/**.cpp",
    "../vendor/GLAD/src/glad.c"
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
    IncludeDirectories["Common"],
    IncludeDirectories["spdlog"],
    IncludeDirectories["GLFW"],
    IncludeDirectories["Glad"],
    IncludeDirectories["GLM"]
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"

filter {}
