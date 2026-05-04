cmake_minimum_required(VERSION 4.0)
project(CrimsonEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../Bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/../_Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/../_Lib/${CMAKE_BUILD_TYPE})
set(CMAKE_CFG_INTDIR ${PROJECT_SOURCE_DIR}/../_Temp/${CMAKE_BUILD_TYPE})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(CrimsonUtilities)

add_subdirectory(CrimsonUtilities)
add_subdirectory(Logging)
add_subdirectory(GraphicsEngine)
add_subdirectory(AssetManager)
add_subdirectory(ModelViewer)