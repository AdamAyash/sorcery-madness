project "Networking"
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
    "Ws2_32",
    "Common"
}

includedirs {
    "%{wks.location}/Common/source",
    "%{wks.location}/vendor/spdlog/include"
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"

filter {}
