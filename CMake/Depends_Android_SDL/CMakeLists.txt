CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

OPTION(MENGINE_USE_PRECOMPILED_HEADER "Mengine use precompiled header" OFF)

PROJECT(Depends_Android_SDL)

INCLUDE(../macro_template.cmake)
INCLUDE(../android_template.cmake)

SET_MENGINE_ENVIRONMENT(PYTHON ANDROID OPENGL X64 SDL GRADLE)

SET(MENGINE_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/../..)
SET(THIRDPARTY_DIR ${MENGINE_REPOSITORY}/dependencies)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

SET(CMAKE_TEMP_DIR ${MENGINE_REPOSITORY}/CMakeTemp/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})

SET(MENGINE_DEPENDENCE_WEBP OFF CACHE BOOL "MENGINE_DEPENDENCE_WEBP" FORCE)

ADD_SUBDIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies ${CMAKE_TEMP_DIR})

