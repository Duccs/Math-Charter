# Create the executable
add_executable(Parser parser-main.cpp)

# Link the library
# Also automatically imports the include paths defined in the library lib-parser.
target_link_libraries(Parser PRIVATE 
    lib-parser
    lib-assist
)

set_target_properties(Parser PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Parser")