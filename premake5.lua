workspace "SorceryMadness"
architecture "x64"
configurations { "Debug", "Release" }
startproject "Client"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

filter "system:windows"
buildoptions { "/utf-8" }

filter "configurations:Debug"
defines { "DEBUG" }

--Dependencies
include "dependencies"

--Common
include "common"

--Networking
include "networking"

-- Client
include "client"

--Server
include "server"

--Engine
include "engine"

group "Dependencies"
include "vendor/GLFW"
group ""
