cmake_minimum_required(VERSION 3.15)

# change to the project's name
project(project)
set(CMAKE_CXX_STANDARD 17)

# Set to the folder containing SDL libraries
# For Windows (MSYS2) and macOS, adjust the paths accordingly
if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/msys64/Libraries")
    set(SDL2_DIR "${CMAKE_PREFIX_PATH}/SDL/lib/cmake/SDL2")
    set(SDL2_image_DIR "${CMAKE_PREFIX_PATH}/SDL_Image/lib/cmake/SDL2_Image")
    set(SDL2_ttf_DIR "${CMAKE_PREFIX_PATH}/SDL_ttf/lib/cmake/SDL2_ttf")
    set(SDL2_mixer_DIR "${CMAKE_PREFIX_PATH}/SDL_mixer/lib/cmake/SDL2_mixer")
    
    find_package(SDL2 REQUIRED CONFIG)
    find_package(SDL2_image REQUIRED CONFIG)
    find_package(SDL2_ttf REQUIRED CONFIG)
    find_package(SDL2_mixer REQUIRED CONFIG)
elseif(APPLE)
    # Use the system-wide SDL2 installation on macOS (or specify custom paths if necessary)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# puts the executable in build/bin
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


# Add flags based on the compiler
if (WIN32)
    # static link to SDL libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer-static)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")  # MinGW/GCC
        target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
    endif()
elseif(APPLE)
    # On macOS, link dynamically to the SDL2 libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer)
endif()