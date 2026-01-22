
# Create the library
add_library(glad STATIC glad.c)

# Make it visible. Includes should match the directory created.
# <glad.h> -> YES
# <glad/glad.h> -> NO
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Linux/Mac fix
# also Untested. Likely need dl install
if (UNIX)
    target_link_libraries(glad PRIVATE dl)
endif()