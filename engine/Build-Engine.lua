project "engine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
	"../dependencies/glfw/include",
	"../dependencies/glew/include",
	"../dependencies/glm"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines
	{
		"GLEW_STATIC",
		"GLCORE_PLATFORM_WINDOWS"
	}

	links 
	{ 
		"../dependencies/glfw/include/../lib/glfw3",
		"../dependencies/glew/include/../lib/x64/Release/glew",
		"opengl32"
	}

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"