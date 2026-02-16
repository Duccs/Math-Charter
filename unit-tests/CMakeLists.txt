# Enable testing
enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include GoogleTest module
include(GoogleTest)

# Collect all test files
file(GLOB_RECURSE TEST_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/**/test_*.cpp"
)

# Create test executable
add_executable(unit_tests 
    test_main.cpp
    ${TEST_SOURCES}
)

# Link against GoogleTest libraries
target_link_libraries(unit_tests
    GTest::gtest
    lib-parser
    # Add other libraries as neccessary
    # curve-lib
    # helper-lib
    # scene-lib
    # shader-lib
)

# Include directories for headers
target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/lib-parser
    ${CMAKE_SOURCE_DIR}/lib-curve
    ${CMAKE_SOURCE_DIR}/lib-assist
    ${CMAKE_SOURCE_DIR}/lib-scene
    ${CMAKE_SOURCE_DIR}/lib-shader
)