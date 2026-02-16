# Create the library
add_library(lib-curve STATIC 
    VertexGenerator.cpp
    VertexGenerator.h
    Curve2d.cpp 
    Curve2d.h
    Line2d.cpp
    Line2d.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-curve PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-curve PUBLIC
    lib-parser
    lib-assist
    glad
    glfw
)