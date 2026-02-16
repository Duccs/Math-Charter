# Create the executable
add_executable(Sole-UI sole-ui-main.cpp)

# Link the library
target_link_libraries(Sole-UI PRIVATE 
    lib-ui
)

set_target_properties(Sole-UI PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Sole-UI")