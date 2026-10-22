workspace "SorceryMadness"
architecture "x64"
configurations { "Debug", "Release" }

-- Client
include "client"

--Server
include "server"
