project "Server"
kind "ConsoleApp"
language "C++"
externalwarnings("Off")

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files {
    "source/**.h",
    "source/**.cpp",
}

links {
    "Networking",
}

includedirs {
    "%{wks.location}/Common/source",
    "%{wks.location}/Networking/source",
    "%{wks.location}/vendor/spdlog/include",
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"
