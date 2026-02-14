# Create the executable
add_executable(parser-test parser-test.cpp)

# Link the library
# Also automatically imports the include paths defined in the library parser-lib.
target_link_libraries(parser-test PRIVATE 
    parser-lib
    helper-lib
)