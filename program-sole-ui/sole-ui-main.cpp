#include <Common-UI.h>
#include <config.h>
#include <assist.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// Forward declarations
// --------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {

    // Initialize GLFW
    // ---------------
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          "Sole-UI", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // VSync

    // Initialize GLAD
    // ---------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // Initialize Dear ImGui
    // ---------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Keyboard nav
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Docking support

    // Color scheme
    ImGui::StyleColorsClassic();

    // Font setup
    float fontSize = 20.0f;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", fontSize);
    io.FontGlobalScale = fontSize / 13.0f;

    // Global UI scale
    // Adjust this if windows/widgets feel too small or too large relative to text.
    ImGui::GetStyle().ScaleAllSizes(fontSize / 30.0f);

    // Platform / Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // Application state
    // -----------------
    // Preferences state
    PreferencesState prefsState;
    LoadPreferences("preferences.cfg", &prefsState);

    // Menubar state
    MenuBarState menuBarState;
    menuBarState.showLog = true;
    menuBarState.showPreferences = true;

    // Log buffer
    std::vector<std::string> logLines;

    // -----------------------------------------------------------------------
    // 5. Render loop
    // -----------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Dockspace
        // ---------
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

        // Menu bar
        // --------
        InitializeMenuBar(window, logLines, &menuBarState);
        
        // Preferences window
        // ------------------
        PreferencesWindow(&menuBarState.showPreferences, &prefsState);

        // Apply preferences to style
        // --------------------------
        ApplyPreferencesStyle(&prefsState);
        
        // Log window
        // ----------
        LogWindow(&menuBarState.showLog, logLines);

        // built-in ImGui Demo and Metrics windows
        // ---------------------------------------
        if (menuBarState.showDemoWindow) ImGui::ShowDemoWindow(&menuBarState.showDemoWindow);
        if (menuBarState.showMetrics)    ImGui::ShowMetricsWindow(&menuBarState.showMetrics);

        // Render
        // ------
        ImGui::Render();

        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(prefsState.bgColor[0], prefsState.bgColor[1], prefsState.bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Save preferences before shutdown
    SavePreferences("preferences.cfg", &prefsState);

    // Cleanup
    // -------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Resize viewport callback
// ------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
