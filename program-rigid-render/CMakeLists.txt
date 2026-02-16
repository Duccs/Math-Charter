# Create the executable
add_executable(Rigid-Render render-main.cpp)

# Link the library
# Also automatically imports the include paths defined in the library lib-scene.
target_link_libraries(Rigid-Render PRIVATE 
    lib-scene
    imgui
)

set_target_properties(Rigid-Render PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Rigid-Render")

# Copy "shaders" to the binary directory
add_custom_target(export_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Rigid-Render/shaders"
    COMMENT "Copying shaders to binary directory..."
)

# Make main depend on the copy task
add_dependencies(Rigid-Render export_shaders)