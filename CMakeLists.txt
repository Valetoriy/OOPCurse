cmake_minimum_required(VERSION 3.23.2)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
# include(CheckIPOSupported)
# check_ipo_supported(RESULT supported OUTPUT error)

project(CURSE LANGUAGES C CXX)

include_directories(extern)

add_executable(${PROJECT_NAME} 
    extern/glad.c
    extern/stb_image.cpp
    src/Shader.cpp
    src/main.cpp)

# set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries(${PROJECT_NAME} PUBLIC
    -fsanitize=address,leak,undefined
    -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
)

target_compile_options(${PROJECT_NAME} PUBLIC
    -fsanitize=address,leak,undefined
    -Og
    # -Ofast
    -Wall -Wextra -Wpedantic -Wconversion
)
