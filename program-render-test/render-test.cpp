#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <helper.h>
#include <shader.h>
#include <graphscene.h>

#include <iostream>
#include <filesystem>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, GraphScene& scene);

// settings
const std::filesystem::path PATH = getPath();
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

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

	// Initialize GLAD (before any OpenGL function ?)
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Create shader program
    // ---------------------
    Shader shader("shader.vs", "shader.fs");

    // Set up graph view size
    // ----------------------
    GraphView myView = {-5.0f, 5.0f, -5.0f, 5.0f};

	// Create scene and add curves
    // ---------------------------
    GraphScene scene(myView, 1.0f);
    Curve2D* curve = scene.addCurve("x", 200);
    curve->setColor(1.0f, 0.0f, 0.0f); // Red parabola
    Curve2D* curve2 = scene.addCurve("n", 200);
    curve2->setColor(0.0f, 0.0f, 1.0f); // Blue inverted parabola
    Curve2D* curve3 = scene.addCurve("s", 200);
    curve3->setColor(0.0f, 1.0f, 0.0f); // Green sine wave
    Curve2D* curve4 = scene.addCurve("c", 200);
    curve4->setColor(1.0f, 1.0f, 0.0f); // Yellow cosine wave

    // Init shader
    shader.use();

	// uncommnet to retrieve maximum number of vertex attributes supported on device
	// int nrAttributes;
	// glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	// std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Render loop
    // -----------
	while(!glfwWindowShouldClose(window)) {

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
        scene.pan(0.0f, 0.004f);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        scene.pan(0.0f, -0.004f);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        scene.pan(-0.004f, 0.0f);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        scene.pan(0.004f, 0.0f);
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        scene.zoom(0.999f);
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        scene.zoom(1.001f);
}

// Resize viewport on call back
// ----------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
