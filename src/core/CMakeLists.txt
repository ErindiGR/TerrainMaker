file(GLOB CORE_SRC_TMC 
"*.cpp"
)

add_definitions("-DGL_SILENCE_DEPRECATION")

add_library(tmcore SHARED ${CORE_SRC_TMC})

target_link_libraries(tmcore "-framework OpenGL")