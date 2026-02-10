# Create the library
add_library(parser-lib STATIC 
    Tokenizer.cpp
    Tokenizer.h
    StateMachine.cpp
    StateMachine.h
    Scanner.cpp
    Scanner.h
)

# Link the library
# Needs to be public... Probably...
target_include_directories(parser-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Throw in other libraries needed. Avoid double throwing cuz why.
target_link_libraries(parser-lib PUBLIC 
    helper-lib
)