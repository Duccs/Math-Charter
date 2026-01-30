# Create the library
add_library(shader-lib STATIC 
    shader.cpp 
    shader.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(shader-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(shader-lib PUBLIC 
    helper-lib
    glad 
    glfw
)