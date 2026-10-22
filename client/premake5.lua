project "Client"
kind "ConsoleApp"
language "C++"

files { "client/source/**.h", "client/source/**.cpp" }

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"
