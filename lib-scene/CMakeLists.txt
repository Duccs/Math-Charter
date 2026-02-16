# Create the library
add_library(lib-scene STATIC 
    Graphscene.cpp 
    Graphscene.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-scene PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-scene PUBLIC 
    lib-curve
    lib-shader
)