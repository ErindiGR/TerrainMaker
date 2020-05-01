cmake_minimum_required(VERSION 3.10)

project(TerrainMaker)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB CORE_SRC_TMC 
"src/core/*.cpp"
)

add_definitions("-DGL_SILENCE_DEPRECATION")

add_library(tmcore SHARED ${CORE_SRC_TMC})




target_link_libraries(tmcore "-framework OpenGL")


include_directories(third_party/includes/)
link_directories(third_party/libs/)
link_directories(third_party/libs/wx)

add_executable(terrain_maker src/ui/main.cpp)

target_include_directories(terrain_maker PUBLIC src/core/)

target_link_libraries(terrain_maker 
    "-framework IOKit"
    "-framework Carbon"
    "-framework Cocoa"
    "-framework AudioToolbox"
    "-framework System"
    "-framework OpenGL"
    "-lwx_osx_cocoau_xrc-3.1"
    "-lwx_osx_cocoau_html-3.1" 
    "-lwx_osx_cocoau_qa-3.1"
    "-lwx_osx_cocoau_core-3.1" 
    "-lwx_baseu_xml-3.1"
    "-lwx_baseu_net-3.1"
    "-lwx_baseu-3.1"
    "-lwx_osx_cocoau_gl-3.1"
    "-lwx_osx_cocoau_propgrid-3.1")

target_link_libraries(terrain_maker tmcore)





