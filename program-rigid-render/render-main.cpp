#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <config.h>
#include <assist.h>
#include <mouse_controller.h>
#include <Shader.h>
#include <Graphscene.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, GraphScene& scene);

// settings
const std::filesystem::path PATH = getPath();

int main() {
	// Initialize glfw, glad, and window
    // ---------------------------------
	printf("PATH directory: %s\n", PATH.string().c_str());
    // Init and Specify version 4.6 (Major 4, Minor 6)
	if(!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Using Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Enable MSAA with 4 samples per pixel (can use 2, 4, 8, or 16)
	glfwWindowHint(GLFW_SAMPLES, 4);	
    // Fix for MacOS X
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Create a window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello, World!", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Callback on user resizing window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Enable VSync (1 = on, 0 = off)
	glfwSwapInterval(1);

	// Initialize GLAD (before any OpenGL function ?)
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Enable MSAA
    // -----------
	glEnable(GL_MULTISAMPLE);

	// Create shader program
    // ---------------------
    Shader shader("shader.vs", "shader.fs");

    // Set up graph view size
    // ----------------------
    GraphView view = {-5.0f, 5.0f, -5.0f, 5.0f};

	// Create scene and add curves
    // ---------------------------
    GraphScene scene(view);

    Curve2D* curve = scene.addCurve("log(x)", 200, 4.0f, {0.0f, 1.0f, 0.0f}); // Red parabola
    curve = scene.addCurve("x^3", 200, 4.0f, {1.0f, 0.0f, 0.0f}); // Blue inverted parabola
    curve = scene.addCurve("sin(x)", 200, 4.0f, {0.0f, 0.0f, 1.0f}); // Green sine wave
    curve = scene.addCurve("cos(x)", 200, 4.0f, {1.0f, 0.7f, 0.0f}); // Yellow cosine wave
    // Curve2D* curve5 = scene.addCurve("tan(x)", 200);
    // curve5->setColor(1.0f, 0.0f, 1.0f); // Magenta tan(x) - tests adaptive sampling!

    // Init shader
    shader.use();

    // Set up mouse controls
    // ----------------------
    MouseController mouse(window, &scene.getView());
    mouse.setPanCallback([&scene](float dx, float dy) {
        scene.pan(dx, dy);
    });
    mouse.setZoomAtCallback([&scene](float worldX, float worldY, float factor) {
        scene.zoomAt(worldX, worldY, factor);
    });
    mouse.install();

	// FPS tracking variables
	// ----------------------
	double lastTime = glfwGetTime();
	double lastFpsTime = lastTime;
	int frameCount = 0;
	double deltaTime = 0.0;

    // Render loop
    // -----------
	while(!glfwWindowShouldClose(window)) {

        // Manual FPS cap -------------------↓↓ change this
        const double targetFrameTime = 1.0 / 30.0;
        while (glfwGetTime() - lastTime < targetFrameTime) {
            // Busy wait or sleep (Thumb twiddling)
        }

		// Calculate delta time and FPS
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		frameCount++;

		// Update FPS counter every second
		if (currentTime - lastFpsTime >= 1.0) {
			double fps = frameCount / (currentTime - lastFpsTime);
			char title[256];
			snprintf(title, sizeof(title), "Math Charter - FPS: %.1f", fps);
			glfwSetWindowTitle(window, title);
			
			frameCount = 0;
			lastFpsTime = currentTime;
		}

		// Input
		processInput(window, scene);

        // Get aspect ratio to render correct distance between coordinates
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspectRatio = (float)width / (float)height;

		// Render
		glClearColor(red(249), green(249), blue(249), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
        scene.render(shader, aspectRatio);

		// Check events and swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    // De-allocate all resources once done
    // -----------------------------------
    scene.cleanup();
    shader.terminate();

    // Clean up resources allocated
    // ----------------------------
	glfwTerminate();
	return 0;
}

// Process keyboardinput
// ---------------------
void processInput(GLFWwindow *window, GraphScene& scene)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        printf("Spacebar pressed\n");
        //createNextCurve();
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        scene.pan(0.0f, 0.07f);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        scene.pan(0.0f, -0.07f);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        scene.pan(-0.07f, 0.0f);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        scene.pan(0.07f, 0.0f);
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        scene.zoom(0.99f);
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        scene.zoom(1.01f);
}

// Resize viewport on call back
// ----------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
