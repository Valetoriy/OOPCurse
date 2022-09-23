cmake_minimum_required(VERSION 3.23.2)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
# include(CheckIPOSupported)
# check_ipo_supported(RESULT supported OUTPUT error)

project(CURSE LANGUAGES C CXX)

add_executable(${PROJECT_NAME} 
    src/glad.c
    src/stb_image.cpp
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
