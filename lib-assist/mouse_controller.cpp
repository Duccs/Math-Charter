#include "mouse_controller.h"

MouseController::MouseController(GLFWwindow* window, const GraphView* view)
    : window(window), view(view) {}

void MouseController::setPanCallback(PanCallback callback) {
    panCallback = std::move(callback);
}

void MouseController::setZoomAtCallback(ZoomAtCallback callback) {
    zoomAtCallback = std::move(callback);
}

void MouseController::install() {
    glfwSetWindowUserPointer(window, this);

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        auto* ctrl = static_cast<MouseController*>(glfwGetWindowUserPointer(w));
        if (ctrl) ctrl->handleMouseButton(button, action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        auto* ctrl = static_cast<MouseController*>(glfwGetWindowUserPointer(w));
        if (ctrl) ctrl->handleCursorPos(xpos, ypos);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        auto* ctrl = static_cast<MouseController*>(glfwGetWindowUserPointer(w));
        if (ctrl) ctrl->handleScroll(xoffset, yoffset);
    });
}

void MouseController::handleMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            dragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        } else if (action == GLFW_RELEASE) {
            dragging = false;
        }
    }
}

void MouseController::handleCursorPos(double xpos, double ypos) {
    if (!dragging || !panCallback) return;

    // Convert both old and new screen positions to world coordinates
    float oldWX, oldWY, newWX, newWY;
    screenToWorld(lastX, lastY, oldWX, oldWY);
    screenToWorld(xpos, ypos, newWX, newWY);

    // Invert: dragging right should move the view left
    float dx = oldWX - newWX;
    float dy = oldWY - newWY;

    panCallback(dx, dy);

    lastX = xpos;
    lastY = ypos;
}

void MouseController::handleScroll(double xoffset, double yoffset) {
    if (!zoomAtCallback) return;

    // Scroll up for zoom in, scroll down for zoom out
    float factor = (yoffset > 0) ? 0.9f : 1.1f;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    float worldX, worldY;
    screenToWorld(cursorX, cursorY, worldX, worldY);

    zoomAtCallback(worldX, worldY, factor);
}

void MouseController::screenToWorld(double sx, double sy, float& wx, float& wy) const {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    // Pixel to NDC [-1, 1]
    float ndcX = static_cast<float>(sx / width) * 2.0f - 1.0f;
    float ndcY = 1.0f - static_cast<float>(sy / height) * 2.0f;

    // Undo shader aspect ratio correction
    if (aspect > 1.0f) {
        ndcY /= aspect;   // Landscape: shader multiplied Y by aspect
    } else {
        ndcX *= aspect;   // Portrait: shader divided X by aspect
    }

    // NDC to world
    wx = view->minX + (ndcX + 1.0f) / 2.0f * (view->maxX - view->minX);
    wy = view->minY + (ndcY + 1.0f) / 2.0f * (view->maxY - view->minY);
}
