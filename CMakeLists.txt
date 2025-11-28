cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_17)

# Add SFML 2.6.1 include and lib paths
target_include_directories(main PRIVATE "E:/SFML-2.6.1/include")
target_link_directories(main PRIVATE "E:/SFML-2.6.1/lib")

# Link SFML 2.x libraries
target_link_libraries(main
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)
