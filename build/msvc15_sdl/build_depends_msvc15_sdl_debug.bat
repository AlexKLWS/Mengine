@echo off

@echo Starting dependencies build debug configuration...

@pushd ..
@call vcvarsall_msvc15
@call build_depends "%CD%\..\CMake\Depends_WIN32_SDL" build_msvc15_sdl_debug "Visual Studio 15 2017" Debug
@popd

@echo Done

@pause