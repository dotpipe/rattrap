cmake_minimum_required(VERSION 3.10)
project(RatTrap VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add source files
set(SOURCES
    main26.cpp
    AnomalyVisualizer.cpp
)

# Add header files
set(HEADERS
    Particles.h
    AnomalyVisualizer.h
)

# Create executable
add_executable(rattrap ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(rattrap
    OpenGL::GL
    GLUT::GLUT
    ${OPENGL_glu_LIBRARY}
    sfml-graphics
    sfml-window
    sfml-system
)

# Install target
install(TARGETS rattrap DESTINATION bin)
