# Create the library
add_library(lib-shader STATIC 
    Shader.cpp 
    Shader.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-shader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-shader PUBLIC 
    lib-assist
    glad
    glfw
)