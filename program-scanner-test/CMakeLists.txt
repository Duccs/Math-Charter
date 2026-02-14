# Create the executable
add_executable(scanner-test scanner-test.cpp)

# Link the library
# Also automatically imports the include paths defined in the library parser-lib.
target_link_libraries(scanner-test PRIVATE 
    parser-lib
    helper-lib
)

# Copy "code.txt" to the binary directory
add_custom_target(copy_code_txt ALL
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/code.txt"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/code.txt"
    COMMENT "Copying code.txt to binary directory..."
)

# Make main depend on the copy task
add_dependencies(scanner-test copy_code_txt)