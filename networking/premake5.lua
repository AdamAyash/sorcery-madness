project "Networking"
kind "StaticLib"
language "C++"
cppdialect "C++23"
externalwarnings("Off")

files {
    "source/**.h",
    "source/**.cpp",
}

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

includedirs {
    IncludeDirectories["Common"],
    IncludeDirectories["spdlog"],
}

filter "system:windows"
links {
    "Ws2_32",
    "Common"
}

filter "system:linux"
links {
    "Common"
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"

filter {}
