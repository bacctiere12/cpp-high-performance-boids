cmake_minimum_required(VERSION 3.14)
project(BoidsSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch SFML from GitHub
include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_MakeAvailable(sfml)
find_package(TBB REQUIRED)

# Add Source files from the src directory
add_executable(Boids 
    src/main.cpp 
    src/boid.cpp 
    src/quadtree.cpp
)

# Include the headers from the include directory
target_include_directories(Boids PRIVATE include)

# Link SFML
target_link_libraries(Boids PRIVATE sfml-graphics sfml-window sfml-system TBB::tbb)

# Windows: Copy DLLs to the build folder
if(WIN32)
    add_custom_command(TARGET Boids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE_DIR:Boids>
    )
endif()