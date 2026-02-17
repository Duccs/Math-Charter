#include "Viewport-UI.h"
#include <cstdio>
#include <cstring>

// ImGui window to display GraphViewport's FBO texture
void GraphViewportWindow(bool* show, GraphViewport& viewport) {
    if (!show || !*show) return;

    // Fill window edge to edge
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Graph Viewport", show,
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImVec2 size = ImGui::GetContentRegionAvail();
    int w = static_cast<int>(size.x);
    int h = static_cast<int>(size.y);

    if (w > 0 && h > 0) {
        // Resize FBO on change
        if (w != viewport.getWidth() || h != viewport.getHeight()) {
            viewport.resize(w, h);
        }

        // Render scene into FBO
        viewport.render();

        // Display FBO texture
        ImTextureID texID = (ImTextureID)(intptr_t)viewport.getTextureID();
        ImGui::Image(texID, size, ImVec2(0, 1), ImVec2(1, 0));

        // Mouse interaction when hovering over the viewport
        if (ImGui::IsItemHovered()) {
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 itemMin  = ImGui::GetItemRectMin();

            // Mouse position in viewport-local pixels
            float mx = mousePos.x - itemMin.x;
            float my = mousePos.y - itemMin.y;

            // Convert to world coordinates
            GraphView& view = viewport.getScene().getView();
            float worldX = view.minX + (mx / size.x) * (view.maxX - view.minX);
            float worldY = view.maxY - (my / size.y) * (view.maxY - view.minY);

            // Scroll wheel → zoom at cursor position
            if (io.MouseWheel != 0.0f) {
                float factor = (io.MouseWheel > 0.0f) ? 0.9f : 1.1f;
                viewport.getScene().zoomAt(worldX, worldY, factor);
            }

            // Left-drag → pan
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                ImVec2 delta = io.MouseDelta;
                float dx = -delta.x / size.x * (view.maxX - view.minX);
                float dy =  delta.y / size.y * (view.maxY - view.minY);
                viewport.getScene().pan(dx, dy);
            }
        }
    } else {
        ImGui::TextDisabled("Viewport too small to render.");
    }

    ImGui::End();
    ImGui::PopStyleVar();
}


void GraphControlPanel(bool* show, GraphViewport& viewport,
                       std::vector<std::string>& logLines, PreferencesState* state) {
    if (!show || !*show) return;

    ImGui::Begin("Graph Controls", show);

    GraphScene& scene = viewport.getScene();
    GraphView   view  = scene.getView(); // copy to detect changes

    // View Bounds
    // -----------
    ImGui::SeparatorText("View Bounds");

    bool viewChanged = false;
    viewChanged |= ImGui::DragFloat("Min X", &view.minX, 0.1f);
    viewChanged |= ImGui::DragFloat("Max X", &view.maxX, 0.1f);
    viewChanged |= ImGui::DragFloat("Min Y", &view.minY, 0.1f);
    viewChanged |= ImGui::DragFloat("Max Y", &view.maxY, 0.1f);

    if (viewChanged) {
        scene.updateView(view);
    }

    if (ImGui::Button("Reset View")) {
        scene.updateView({-5.0f, 5.0f, -5.0f, 5.0f});
        logLines.push_back("[Graph] View reset to default");
    }

    // Viewport Background
    // -------------------
    ImGui::SeparatorText("Viewport Background");
    ImGui::ColorEdit3("Graph BG", viewport.getBgColor());

    // Curves
    // ------
    ImGui::SeparatorText("Curves");

    size_t curveCount = scene.getCurveCount();
    int removeIndex = -1; // deferred removal

    for (size_t i = 0; i < curveCount; i++) {
        Curve2D* curve = scene.getCurve(i);
        if (!curve) continue;

        ImGui::PushID(static_cast<int>(i));

        char label[128];
        snprintf(label, sizeof(label), "%s", curve->getEquation().c_str());

        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            if (state->showAdvancedSettings) {

                // Color picker
                RenderColor col = curve->getColor();
                float c[3] = {col.red, col.green, col.blue};
                if (ImGui::ColorEdit3("Color", c)) {
                    curve->setColor(c[0], c[1], c[2]);
                }

                // Line width
                float lw = curve->getLineWidth();
                if (ImGui::SliderFloat("Line Width", &lw, 0.5f, 10.0f)) {
                    curve->setLineWidth(lw);
                }

                // Point count
                int np = curve->getNumPoints();
                if (ImGui::SliderInt("Points", &np, 10, 1000)) {
                    curve->setNumPoints(np);
                    curve->update(scene.getView());
                }

                // Red color for remove button
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));           // Normal
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));   // Hovered
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
                // Remove buttons
                if (ImGui::Button("Remove")) {
                    removeIndex = static_cast<int>(i);
                }
                ImGui::PopStyleColor(3); // Pop all three pushed style colors
            } else {

                ImGui::Dummy(ImVec2(0.0f, 5.0f));

                // Predefined color palette
                RenderColor curCol = curve->getColor();
                for (int ci = 0; ci < 6; ci++) {
                    ImGui::PushID(ci);
                    const ImVec4& pc = curvePalette[ci];
                    bool selected = (curCol.red == pc.x && curCol.green == pc.y && curCol.blue == pc.z);
                    if (selected) {
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
                    }
                    if (ImGui::ColorButton("##swatch", pc, ImGuiColorEditFlags_NoTooltip, ImVec2(40, 40))) {
                        curve->setColor(pc.x, pc.y, pc.z);
                    }
                    if (selected) {
                        ImGui::PopStyleColor();
                        ImGui::PopStyleVar();
                    }
                    if (ci < 7) ImGui::SameLine();
                    ImGui::PopID();
                }
                
                ImGui::Dummy(ImVec2(0.0f, 5.0f));

                // Line type selector
                const char* typeLabels[] = { "Straight", "Dashed", "Dotted" };
                int currentType = static_cast<int>(curve->getLineType());
                for (int t = 0; t < 3; t++) {
                    if (t > 0) ImGui::SameLine();
                    bool active = (currentType == t);
                    if (active) {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
                    }
                    if (ImGui::Button(typeLabels[t])) {
                        curve->setLineType(static_cast<LineType>(t));
                    }
                    if (active) {
                        ImGui::PopStyleColor();
                    }
                }

                ImGui::SameLine();

                // Visibility toggle
                bool vis = curve->isVisible();
                if (ImGui::Checkbox("Visible", &vis)) {
                    curve->setVisible(vis);
                }

                ImGui::Dummy(ImVec2(0.0f, 5.0f));
            }
        } else {
        }

        ImGui::PopID();
    }

    // Apply deferred removal
    if (removeIndex >= 0) {
        Curve2D* curve = scene.getCurve(static_cast<size_t>(removeIndex));
        if (curve) {
            logLines.push_back("[Graph] Removed curve: " + curve->getEquation());
            scene.removeCurve(curve);
        }
    }

    // Add Curve
    // ---------
    ImGui::SeparatorText("Add Curve");

    static char  newEquation[256] = "";
    static float newColor[3]     = {1.0f, 0.0f, 0.0f};
    static float newLineWidth    = 4.0f;
    static int   newPoints       = 200;

    ImGui::InputText("Equation", newEquation, sizeof(newEquation));
    ImGui::ColorEdit3("Color##new", newColor);
    ImGui::SliderFloat("Line Width##new", &newLineWidth, 0.5f, 10.0f);
    ImGui::SliderInt("Points##new", &newPoints, 10, 1000);

    if (ImGui::Button("Add Curve") && strlen(newEquation) > 0) {
        RenderColor color = {newColor[0], newColor[1], newColor[2]};
        scene.addCurve(newEquation, newPoints, newLineWidth, color);
        logLines.push_back(std::string("[Graph] Added curve: ") + newEquation);
        newEquation[0] = '\0';
    }

    ImGui::End();
}
