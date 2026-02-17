#include <MenuBar-UI.h>
#include <Preferences-UI.h>
#include <Viewport-UI.h>
#include <Common-UI.h>
#include <config.h>
#include <assist.h>
#include <imgui_internal.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <filesystem>

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
    io.ConfigWindowsMoveFromTitleBarOnly = true;          // Restrict window moving to title bar

    // Color scheme
    ImGui::StyleColorsClassic();

    // Font setup — load bundled fonts from the executable's directory
    float fontSize = 20.0f;
    
    std::vector<FontDef> fonts = retrieveFonts();
    int fontCount = fonts.size();

    std::vector<ImFont*> fontArray(fontCount);
    std::vector<std::string> fontNames(fontCount);
    for (int i = 0; i < fontCount; i++) {
        FontDef font = fonts[i];
        fontArray[i] = io.Fonts->AddFontFromFileTTF(font.filename.c_str(), fontSize);
        fontNames[i] = font.name;
        if (!fontArray[i]) {
            printf("[Font] Could not load %s — using ImGui default\n", font.filename.c_str());
            fontArray[i] = io.Fonts->AddFontDefault();
        }
    }
    int FONT_COUNT = fontCount;

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

    // Populate font names into preferences
    int safeFontCount = (FONT_COUNT > PreferencesState::MAX_FONTS) ? PreferencesState::MAX_FONTS : FONT_COUNT;
    for (int i = 0; i < safeFontCount; i++) {
        prefsState.fontNames[i] = fontNames[i].c_str();
    }
    if (FONT_COUNT > PreferencesState::MAX_FONTS) {
        printf("[Warning] Loaded %d fonts, but only %d can be displayed in preferences (MAX_FONTS limit)\n", 
               FONT_COUNT, PreferencesState::MAX_FONTS);
    }
    prefsState.fontCount = safeFontCount;
    // Clamp saved fontCombo to valid range
    if (prefsState.fontCombo < 0 || prefsState.fontCombo >= safeFontCount)
        prefsState.fontCombo = 0;

    // Menubar state
    MenuBarState menuBarState;
    menuBarState.showLog = true;
    menuBarState.showPreferences = true;
    menuBarState.showViewport = false;
    menuBarState.showGraphControls = false;

    // Log buffer
    std::vector<std::string> logLines;

    // Graph viewport
    // --------------
    GraphViewport viewport({-5.0f, 5.0f, -5.0f, 5.0f});
    if (!viewport.init()) {
        logLines.push_back("[Error] Failed to initialize graph viewport shaders");
    } else {
        viewport.getScene().addCurve("e^(1/x)", 200, 4.0f, {0.0f, 1.0f, 0.0f});
        viewport.getScene().addCurve("x^3",    200, 4.0f, {1.0f, 0.0f, 0.0f});
        viewport.getScene().addCurve("sin(x)", 200, 4.0f, {0.0f, 0.0f, 1.0f});
        viewport.getScene().addCurve("cos(x)", 200, 4.0f, {1.0f, 0.7f, 0.0f});
        logLines.push_back("[Graph] Viewport initialized with default curves");
    }

    // Render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Apply selected font
        int fontIdx = (prefsState.fontCombo >= 0 && prefsState.fontCombo < FONT_COUNT)
                      ? prefsState.fontCombo : 0;
        if (fontIdx >= 0 && fontIdx < (int)fontArray.size() && fontArray[fontIdx] != nullptr) {
            ImGui::PushFont(fontArray[fontIdx]);
        }

        // Dockspace
        // ---------
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

        // Setup docking layout on first run
        static bool first_time = true;
        if (first_time) {
            first_time = false;
            // Only set up default layout if config file doesn't exist
            std::filesystem::path configPath("imgui.ini");
            if (!std::filesystem::exists(configPath)) {
                DockLayout layout = SetupDockingLayout(dockspace_id);
                
                ImGui::DockBuilderDockWindow("Preferences", layout.right_upper);
                ImGui::DockBuilderDockWindow("Log", layout.right_lower);
            }
        }

        ImGui::DockSpaceOverViewport(dockspace_id, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

        // Menu bar
        // --------
        InitializeMenuBar(window, logLines, &menuBarState);
        
        // Preferences window
        // ------------------
        PreferencesWindow(&menuBarState.showPreferences, &prefsState);

        // Apply preferences to style
        // --------------------------
        ApplyPreferences(&prefsState);
        
        // Graph viewport window
        // ---------------------
        GraphViewportWindow(&menuBarState.showViewport, viewport);

        // Graph control panel
        // -------------------
        GraphControlPanel(&menuBarState.showGraphControls, viewport, logLines, &prefsState);

        // Log window
        // ----------
        LogWindow(&menuBarState.showLog, logLines);

        // built-in ImGui Demo and Metrics windows
        // ---------------------------------------
        if (menuBarState.showDemoWindow) ImGui::ShowDemoWindow(&menuBarState.showDemoWindow);
        if (menuBarState.showMetrics)    ImGui::ShowMetricsWindow(&menuBarState.showMetrics);

        // Pop font
        if (fontIdx >= 0 && fontIdx < (int)fontArray.size() && fontArray[fontIdx] != nullptr) {
            ImGui::PopFont();
        }

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
