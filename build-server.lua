-- premake5.lua
workspace "server"
architecture "x64"
configurations {"Debug", "Release", "Dist"}
startproject "PixelChase-Server"

-- Workspace-wide defines
defines {"WL_HEADLESS"}

-- Workspace-wide build options for MSVC
filter "system:windows"
buildoptions {"/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/utf-8"}

-- Directories
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
WalnutNetworkingBinDir =
    "external/Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "external/Walnut/Build-Walnut-Headless-External.lua"

group "App"
include "common/build-app-common-headless.lua"
include "server/build-app-server-headless.lua"
group ""
