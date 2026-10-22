project "Client"
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
defines { "DEBUG" }
symbols "On"

filter "configurations:Release"
optimize "On"
