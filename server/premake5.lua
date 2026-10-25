project "Server"
kind "ConsoleApp"
language "C++"

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
