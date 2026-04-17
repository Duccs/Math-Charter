# Create the library
add_library(lib-scanner STATIC 
    Tokenizer.cpp
    Tokenizer.h
    StateMachine.cpp
    StateMachine.h
    Scanner.cpp
    Scanner.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(lib-scanner PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(lib-scanner PUBLIC 
    lib-assist
)