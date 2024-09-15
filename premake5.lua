workspace "GTA_VC_DisableBackfaceCulling"
	configurations { "Release" }
	location "build"

	files { "src/*.*" }

	includedirs { "src" }
	  
project "GTA_VC_DisableBackfaceCulling"
	kind "SharedLib"
	language "C++"
	targetname "disablebackfaceculling"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".asi"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		symbols "On"
		flags { "StaticRuntime" }