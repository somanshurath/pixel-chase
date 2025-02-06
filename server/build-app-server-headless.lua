project "PixelChase-Server"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
targetdir "bin/%{cfg.buildcfg}"
staticruntime "off"

files {"src/**.h", "src/**.cpp"}

includedirs {"../common/src", "../external/Walnut/vendor/glm", "../external/Walnut/Walnut/Source",
             "../external/Walnut/Walnut/Platform/Headless", "../external/Walnut/vendor/spdlog/include",
             "../external/Walnut/vendor/yaml-cpp/include", -- Walnut-Networking
"../external/Walnut/Walnut-Modules/Walnut-Networking/Source",
             "../external/Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/include"}

links {"PixelChase-Common-Headless", "Walnut-Headless", "Walnut-Networking", "yaml-cpp"}

defines {"YAML_CPP_STATIC_DEFINE"}

targetdir("../bin/" .. outputdir .. "/%{prj.name}")
objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

filter "system:windows"
systemversion "latest"
defines {"WL_PLATFORM_WINDOWS"}

postbuildcommands {'{COPY} "../%{WalnutNetworkingBinDir}/GameNetworkingSockets.dll" "%{cfg.targetdir}"',
                   '{COPY} "../%{WalnutNetworkingBinDir}/libcrypto-3-x64.dll" "%{cfg.targetdir}"',
                   '{COPY} "../%{WalnutNetworkingBinDir}/libprotobufd.dll" "%{cfg.targetdir}"'}

filter "system:linux"
libdirs {"../external/Walnut/Walnut-Networking/vendor/GameNetworkingSockets/bin/Linux"}
links {"GameNetworkingSockets"}

defines {"WL_HEADLESS"}

filter "configurations:Debug"
defines {"WL_DEBUG"}
runtime "Debug"
symbols "On"

filter "configurations:Release"
defines {"WL_RELEASE"}
runtime "Release"
optimize "On"
symbols "On"

filter "configurations:Dist"
defines {"WL_DIST"}
runtime "Release"
optimize "On"
symbols "Off"
