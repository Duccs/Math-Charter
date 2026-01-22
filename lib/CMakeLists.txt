include(FetchContent)

# Fetch GLFW
# ----------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4   # Arbitrary version?
)

# Silence GLFW's own build process
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# Add GLAD
# --------
# Assumes glad folder is inside 'lib/'
add_subdirectory(glad)

# Fetch DearImGui
# ---------------
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.92.5
)
FetchContent_MakeAvailable(imgui)
message(STATUS "ImGui is located at: '${imgui_SOURCE_DIR}'")

add_library(imgui STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC 
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(imgui PRIVATE glfw)


# Unit tests could be built separetly for less headache and faster builds

# # Fetch Catch2
# # - Declare
# FetchContent_Declare(
# 	Catch2
# 	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
# 	GIT_TAG v3.12.0 # replace with latest
# )

# # - Download
# FetchContent_MakeAvailable(Catch2)

# # - Create Executable
# add_executable(unit_tests tests/test_logic.cpp)
# target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)