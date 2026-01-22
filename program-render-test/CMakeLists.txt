# Create the executable
add_executable(render-test render-test.cpp)

# Link the library
# Also automatically imports the include paths defined in the library shader-lib.
target_link_libraries(render-test PRIVATE shader-lib)