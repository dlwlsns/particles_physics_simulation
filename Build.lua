-- premake5.lua
workspace "particles_physics_simulation"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "client"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "engine/Build-Engine.lua"
include "client/Build-Client.lua"