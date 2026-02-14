# Create the executable
add_executable(render-test render-test.cpp)

# Link the library
# Also automatically imports the include paths defined in the library shader-lib.
target_link_libraries(render-test PRIVATE 
    scene-lib
    imgui
)

# Copy "shaders" to the binary directory
add_custom_target(export_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders"
    COMMENT "Copying shaders to binary directory..."
)

# Make main depend on the copy task
add_dependencies(render-test export_shaders)