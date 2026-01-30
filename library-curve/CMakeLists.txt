# Create the library
add_library(curve-lib STATIC 
    curve2d.cpp 
    curve2d.h
    curves.cpp 
    curves.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(curve-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(curve-lib PUBLIC
    helper-lib
    glad
    glfw
)