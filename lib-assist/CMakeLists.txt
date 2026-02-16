# Create the library
add_library(lib-assist STATIC 
    assist.cpp 
    assist.h
    config.h
    mouse_controller.cpp
    mouse_controller.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-assist PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-assist PUBLIC glfw)