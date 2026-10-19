workspace "SorceryMadness"
configurations { "Debug", "Release" }

project "Client"
    kind "ConsoleApp"
    language "C++"

    files { "client/source/**.h", "client/source/**.cpp" }

    filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
    
    filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

