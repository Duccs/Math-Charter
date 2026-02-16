#ifndef _GRAPH_VIEWPORT_H_
#define _GRAPH_VIEWPORT_H_

#include <Graphscene.h>
#include <Shader.h>
#include <glad/glad.h>

// Renders a GraphScene into an off-screen framebuffer object
// Resulting texture is displayed in an ImGui window using ImGui::Image()
class GraphViewport {
    private:
        // FBO resources
        GLuint fbo = 0;
        GLuint colorTexture = 0;
        GLuint depthRbo = 0;
        int fbWidth = 0;
        int fbHeight = 0;

        // Scene and shader
        GraphScene scene;
        Shader* shader = nullptr;
        bool initialized = false;

        // Viewport settings
        float bgColor[3] = {0.976f, 0.976f, 0.976f}; // Light gray (249/255)

        // Internal
        void createFramebuffer(int w, int h);
        void deleteFramebuffer();

    public:
        GraphViewport(GraphView initialView);
        ~GraphViewport();

        // Non-copyable (owns GL resources)
        GraphViewport(const GraphViewport&) = delete;
        GraphViewport& operator=(const GraphViewport&) = delete;

        // Initialize shaders.
        bool init();

        // Resize the FBO.
        void resize(int w, int h);

        // Render the scene to the FBO. Saves/restores GL state.
        void render();

        // Accessors
        GLuint getTextureID() const { return colorTexture; }
        int getWidth()  const { return fbWidth; }
        int getHeight() const { return fbHeight; }

        GraphScene&       getScene()       { return scene; }
        const GraphScene& getScene() const { return scene; }

        float*       getBgColor()       { return bgColor; }
        const float* getBgColor() const { return bgColor; }
        void setBgColor(float r, float g, float b);
};

#endif /* _GRAPH_VIEWPORT_H_ */
