cmake_minimum_required(VERSION 3.15)
project(AsciiVideo C)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable verbose output during compilation (optional)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set the build output directory for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)

# Define source and include directories
file(GLOB SOURCES "src/*.c")

# Add platform-specific configurations
if(WIN32)
    message(STATUS "Windows isn't supported till now.")
else()
    message(STATUS "Configuring for Unix-like system")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED
        libavcodec libavdevice libavfilter libavformat libavutil libswresample libswscale)
    include_directories(${FFMPEG_INCLUDE_DIRS})
    link_directories(${FFMPEG_LIBRARY_DIRS})
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})

endif()

# Define the target
add_executable(asciivideo ${SOURCES})

# Link the libraries
if(WIN32)
    target_link_directories(asciivideo PRIVATE ${FFMPEG_LIBRARY_DIR})
    target_link_libraries(asciivideo ${FFMPEG_LIBRARIES})
else()
    target_link_libraries(asciivideo ${FFMPEG_LIBRARIES} ${CURSES_LIBRARIES} m)
endif()

# Install rules (optional)
install(TARGETS asciivideo DESTINATION bin)
