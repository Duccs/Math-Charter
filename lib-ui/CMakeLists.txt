# Create the library
add_library(lib-ui STATIC 
    Common-UI.cpp 
    Common-UI.h
    Assist-UI.cpp
    Assist-UI.h
    MenuBar-UI.cpp
    MenuBar-UI.h
    Preferences-UI.cpp
    Preferences-UI.h
    Viewport-UI.cpp
    Viewport-UI.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-ui PUBLIC 
    lib-assist
    lib-scene
    glad
    glfw
    imgui
)