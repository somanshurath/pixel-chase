-- premake5.lua
workspace "client"
architecture "x64"
configurations {"Debug", "Release", "Dist"}
startproject "PixelChase-Client"

-- Workspace-wide build options for MSVC
filter "system:windows"
buildoptions {"/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/utf-8"}

-- Directories
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
WalnutNetworkingBinDir =
    "external/Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "external/Walnut/Build-Walnut-External.lua"

group "App"
include "common/build-app-common.lua"
include "client/build-app-client.lua"
group ""
