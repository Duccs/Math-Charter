# Create the executable
add_executable(render-test render-test.cpp)

# Link the library
# Also automatically imports the include paths defined in the library shader-lib.
target_link_libraries(render-test PRIVATE 
    scene-lib    
)

# Copy "shaders" folder to the build directory
add_custom_command(TARGET render-test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
    COMMENT "Copying shaders to binary directory..."
)