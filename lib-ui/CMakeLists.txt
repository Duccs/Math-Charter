# Create the library
add_library(lib-ui STATIC 
    Common-UI.cpp 
    Common-UI.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-ui PUBLIC 
    lib-assist
    glad
    glfw
    imgui
)