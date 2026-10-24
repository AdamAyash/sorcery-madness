project "Client"
kind "ConsoleApp"
language "C++"

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
    "%{wks.location}/Networking/source"
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"
