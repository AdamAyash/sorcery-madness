workspace "SorceryMadness"
architecture "x64"
configurations { "Debug", "Release" }
startproject "Client"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
buildoptions { "/utf-8" }
--Common
include "common"

--Networking
include "networking"

-- Client
include "client"

--Server
include "server"
