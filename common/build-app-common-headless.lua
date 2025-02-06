project "PixelChase-Common-Headless"
kind "StaticLib"
language "C++"
cppdialect "C++20"
targetdir "bin/%{cfg.buildcfg}"
staticruntime "off"

files {"src/**.h", "src/**.cpp"}

includedirs {"../external/Walnut/vendor/glm", "../external/Walnut/Walnut/Source",
             "../external/Walnut-Networking/Source", "../external/Walnut/vendor/spdlog/include",
             "../external/vendor/GameNetworkingSockets/include"}

links {"Walnut-Headless", "Walnut-Networking"}

targetdir("../bin/" .. outputdir .. "/%{prj.name}")
objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

filter "system:windows"
systemversion "latest"
defines {"WL_PLATFORM_WINDOWS"}

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
