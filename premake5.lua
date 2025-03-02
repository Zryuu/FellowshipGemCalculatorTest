-- premake5.lua
workspace "FGCTest"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "FGCTest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "WalnutApp"