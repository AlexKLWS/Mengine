@echo off

if ["%~1"]==[""] (
  @echo invalid arguments, please select configuration
  goto end
)

set "CONFIGURATION=%1"

@echo Starting make solution tools %CONFIGURATION% configuration...

set "VERSION=15"
set "YEAR=2017"

@pushd %~dp0..
@call vcvarsall_msvc%VERSION%.bat
@popd

@pushd %~dp0..
@call make_solution.bat "SOURCE_DIRECTORY=%CD%\..\CMake\Tools_Win32" "SOLUTION_NAME=solution_tools_msvc%VERSION%" "GENERATOR=Visual Studio %VERSION% %YEAR%" "ARCHITECTURE=Win32" "CONFIGURATION=%CONFIGURATION%" "LIBRARY_DIRECTORY=build_msvc%VERSION%\%CONFIGURATION%"
@popd

:end
@echo Done

@pause

