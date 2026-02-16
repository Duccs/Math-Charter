#ifndef _MOUSE_CONTROLLER_H_
#define _MOUSE_CONTROLLER_H_

#include <GLFW/glfw3.h>
#include <assist.h>
#include <functional>

class MouseController {

    public:
        using PanCallback = std::function<void(float dx, float dy)>;
        using ZoomAtCallback = std::function<void(float worldX, float worldY, float factor)>;

        MouseController(GLFWwindow* window, const GraphView* view);

        void setPanCallback(PanCallback callback);
        void setZoomAtCallback(ZoomAtCallback callback);

        // Register GLFW callbacks on the window
        void install();

        // Internal handlers (called by static GLFW callbacks)
        void handleMouseButton(int button, int action, int mods);
        void handleCursorPos(double xpos, double ypos);
        void handleScroll(double xoffset, double yoffset);

    private:
        GLFWwindow* window;
        const GraphView* view;

        bool dragging = false;
        double lastX = 0.0;
        double lastY = 0.0;

        PanCallback panCallback;
        ZoomAtCallback zoomAtCallback;

        // Convert screen pixel coordinates to world coordinates
        void screenToWorld(double sx, double sy, float& wx, float& wy) const;
};

#endif /* _MOUSE_CONTROLLER_H_ */
