# Create the library
add_library(helper-lib STATIC 
    helper.cpp 
    helper.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(helper-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(helper-lib PUBLIC)