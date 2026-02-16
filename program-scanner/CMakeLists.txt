# Create the executable
add_executable(Scanner scanner-main.cpp)

# Link the library
# Also automatically imports the include paths defined in the library lib-parser.
target_link_libraries(Scanner PRIVATE 
    lib-parser
    lib-assist
)

set_target_properties(Scanner PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Scanner")

# Copy "code.txt" to the binary directory
add_custom_target(copy_code_txt ALL
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/code.txt"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Scanner/code.txt"
    COMMENT "Copying code.txt to binary directory..."
)

# Make main depend on the copy task
add_dependencies(Scanner copy_code_txt)