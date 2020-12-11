add_library(tmexp SHARED exp.cpp)
target_include_directories(tmexp PUBLIC ../core/)
target_include_directories(tmexp PUBLIC ../../third_party/includes/stb_image/)
target_link_libraries(tmexp tmcore)