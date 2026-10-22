project "Server"
kind "ConsoleApp"
language "C++"

files {
    "source/**.h",
    "source/**.cpp",
}

links {
    "Ws2_32",
    "Mswsock",
    "AdvApi32"
}

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"
