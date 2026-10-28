project "Common"
kind "StaticLib"
language "C++"
cppdialect "C++20"
externalwarnings("Off")

files {
    "source/**.h",
    "source/**.cpp",
}

includedirs {
    "%{wks.location}/vendor/spdlog/include",
}

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

filter "system:windows"

filter "configurations:Debug"
defines {
    "DEBUG",
}
symbols "On"

filter "configurations:Release"
optimize "On"
