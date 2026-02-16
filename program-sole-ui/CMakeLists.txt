# Create the executable
add_executable(Sole-UI sole-ui-main.cpp)

# Link the library
target_link_libraries(Sole-UI PRIVATE 
    lib-ui
)

set_target_properties(Sole-UI PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Sole-UI")

# Copy shaders to binary directory
add_custom_target(export_shaders_sole_ui ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/program-rigid-render/shaders"
        "${CMAKE_SOURCE_DIR}/bin/Sole-UI/shaders"
    COMMENT "Copying shaders to Sole-UI binary directory..."
)
add_dependencies(Sole-UI export_shaders_sole_ui)

# Copy bundled fonts to binary directory
add_custom_target(export_fonts_sole_ui ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources/fonts"
        "${CMAKE_SOURCE_DIR}/bin/Sole-UI/fonts"
    COMMENT "Copying fonts to Sole-UI binary directory..."
)
add_dependencies(Sole-UI export_fonts_sole_ui)