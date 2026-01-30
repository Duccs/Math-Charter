# Create the library
add_library(scene-lib STATIC 
    graphscene.cpp 
    graphscene.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(scene-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(scene-lib PUBLIC 
    curve-lib
    shader-lib
)