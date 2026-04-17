#include "Viewport-UI.h"
#include <cstdio>
#include <cstring>
#include <set>

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
                       std::vector<std::string>& logLines, PreferencesState* state,
                       float deltaTime) {
    if (!show || !*show) return;

    ImGui::Begin("Graph Controls", show);

    GraphScene& scene = viewport.getScene();
    GraphView   view  = scene.getView(); // copy to detect changes
    auto& animStates = getConstantAnimStates();

    // New Curve Section
    // -----------------
    ImGui::SeparatorText("New Curve");

    static char  newEquation[256] = "";
    static float newColor[3]     = {1.0f, 0.0f, 0.0f};
    static float newLineWidth    = 4.0f;
    static char  addError[256]   = "";

    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputTextWithHint("##NewEquation", "Enter equation (e.g., y=sin(x), a=3)", 
                                  newEquation, sizeof(newEquation), 
                                  ImGuiInputTextFlags_EnterReturnsTrue)) {
        // Submit on Enter
        if (strlen(newEquation) > 0) {
            RenderColor color = {newColor[0], newColor[1], newColor[2]};
            try {
                Curve2D* result = scene.addCurve(newEquation, newLineWidth, color);
                if (result) {
                    logLines.push_back(std::string("[Graph] Added curve: ") + newEquation);
                } else {
                    logLines.push_back(std::string("[Graph] Added constant: ") + newEquation);
                }
                addError[0] = '\0';
                newEquation[0] = '\0';
            } catch (const std::exception& e) {
                snprintf(addError, sizeof(addError), "%s", e.what());
                logLines.push_back(std::string("[Error] ") + e.what());
            }
        }
    }

    // Show error inline
    if (addError[0] != '\0') {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        ImGui::TextWrapped("%s", addError);
        ImGui::PopStyleColor();
    }

    // Color palette for new curve
    ImGui::Text("Color:");
    ImGui::SameLine();
    for (int ci = 0; ci < 6; ci++) {
        ImGui::PushID(ci + 1000);
        const ImVec4& pc = curvePalette[ci];
        bool selected = (newColor[0] == pc.x && newColor[1] == pc.y && newColor[2] == pc.z);
        if (selected) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
        }
        if (ImGui::ColorButton("##swatch", pc, ImGuiColorEditFlags_NoTooltip, ImVec2(24, 24))) {
            newColor[0] = pc.x; newColor[1] = pc.y; newColor[2] = pc.z;
        }
        if (selected) {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }
        ImGui::SameLine();
        ImGui::PopID();
    }
    ImGui::NewLine();

    if (state->showAdvancedSettings) {
        ImGui::SliderFloat("Line Width##new", &newLineWidth, 0.5f, 10.0f);
    }

    // View Bounds Section
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
    if (state->showAdvancedSettings) {
        ImGui::SeparatorText("Viewport Background");
        ImGui::ColorEdit3("Graph BG", viewport.getBgColor());
    }

    // Active Curves and Constants
    // ---------------------------
    size_t curveCount = scene.getCurveCount();
    const SymbolTable& constants = scene.getConstants();
    size_t constCount = constants.GetCount();
    
    char sectionLabel[64];
    snprintf(sectionLabel, sizeof(sectionLabel), "Active Items (%zu)", curveCount + constCount);
    ImGui::SeparatorText(sectionLabel);

    // Batch action toolbar
    if (curveCount > 0 || constCount > 0) {
        if (ImGui::SmallButton("Show All")) {
            for (size_t i = 0; i < curveCount; i++) {
                if (Curve2D* c = scene.getCurve(i)) c->setVisible(true);
            }
            logLines.push_back("[Graph] All curves shown");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Hide All")) {
            for (size_t i = 0; i < curveCount; i++) {
                if (Curve2D* c = scene.getCurve(i)) c->setVisible(false);
            }
            logLines.push_back("[Graph] All curves hidden");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Stop All")) {
            for (auto& [name, anim] : animStates) {
                anim.isPlaying = false;
            }
            logLines.push_back("[Graph] All animations stopped");
        }
    }

    int removeIndex = -1;
    std::string removeConstant;

    // Display Constants
    // -----------------
    std::set<std::string> constantNamesSet = constants.GetAllVariables();
    std::vector<std::string> constantNames(constantNamesSet.begin(), constantNamesSet.end());
    for (size_t ci = 0; ci < constantNames.size(); ci++) {
        const std::string& name = constantNames[ci];
        float value = constants.GetValue(name);

        ImGui::PushID(static_cast<int>(ci + 10000)); // unique ID range for constants

        // Get or create animation state
        if (animStates.find(name) == animStates.end()) {
            animStates[name] = ConstantAnimState{};
        }
        ConstantAnimState& anim = animStates[name];

        // Header with constant icon
        char constLabel[128];
        snprintf(constLabel, sizeof(constLabel), "[Const] %s = %.3f", name.c_str(), value);
        
        if (ImGui::CollapsingHeader(constLabel, ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(10.0f);

            // Current value (editable)
            float editValue = value;
            ImGui::SetNextItemWidth(120);
            if (ImGui::DragFloat("Value", &editValue, 0.01f)) {
                scene.setConstant(name, editValue);
            }

            ImGui::Dummy(ImVec2(0, 5));

            // Animation controls
            ImGui::Text("Animation:");
            
            // Play/Pause button
            bool wasPlaying = anim.isPlaying;  // Store state before button click
            const char* playLabel = wasPlaying ? "Pause" : "Play";
            if (wasPlaying) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
            }
            if (ImGui::Button(playLabel)) {
                anim.isPlaying = !anim.isPlaying;
                if (anim.isPlaying) {
                    logLines.push_back("[Graph] Animation started: " + name);
                }
            }
            if (wasPlaying) {  // Pop based on state before button click
                ImGui::PopStyleColor(2);
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset##anim")) {
                float midpoint = (anim.minValue + anim.maxValue) / 2.0f;
                scene.setConstant(name, midpoint);
                anim.direction = 1;
            }

            // Min/Max/Speed controls
            ImGui::SetNextItemWidth(80);
            ImGui::DragFloat("Min", &anim.minValue, 0.1f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(80);
            ImGui::DragFloat("Max", &anim.maxValue, 0.1f);
            
            ImGui::SetNextItemWidth(120);
            ImGui::SliderFloat("Speed", &anim.speed, 0.1f, 10.0f, "%.1f/s");

            // Tick animation
            if (anim.isPlaying && deltaTime > 0.0f) {
                float newValue = value + anim.direction * anim.speed * deltaTime;
                
                // Ping-pong at boundaries
                if (newValue >= anim.maxValue) {
                    newValue = anim.maxValue;
                    anim.direction = -1;
                } else if (newValue <= anim.minValue) {
                    newValue = anim.minValue;
                    anim.direction = 1;
                }
                
                scene.setConstant(name, newValue);
                
                // Regenerate all curves to reflect the new constant value
                for (size_t ci = 0; ci < scene.getCurveCount(); ci++) {
                    Curve2D* c = scene.getCurve(ci);
                    if (c && c->isVisible()) {
                        try {
                            c->setConstants(&scene.getConstants());
                            c->update(scene.getView());
                        } catch (...) {
                            // Curve has an error, will be caught elsewhere
                        }
                    }
                }
            }

            ImGui::Dummy(ImVec2(0, 5));

            // Remove button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));
            if (ImGui::Button("Remove##const")) {
                removeConstant = name;
            }
            ImGui::PopStyleColor(3);

            ImGui::Unindent(10.0f);
        }

        ImGui::PopID();
    }


    // Display Curves
    // --------------
    // Storage for inline equation editing
    static std::unordered_map<size_t, char[256]> editBuffers;
    static std::unordered_map<size_t, char[256]> editErrors;

    for (size_t i = 0; i < curveCount; i++) {
        Curve2D* curve = scene.getCurve(i);
        if (!curve) continue;

        ImGui::PushID(static_cast<int>(i));

        // Initialize edit buffer if needed
        if (editBuffers.find(i) == editBuffers.end()) {
            strncpy(editBuffers[i], curve->getEquation().c_str(), 255);
            editBuffers[i][255] = '\0';
            editErrors[i][0] = '\0';
        }

        // Curve header with color indicator
        RenderColor col = curve->getColor();
        ImVec4 headerCol(col.red, col.green, col.blue, 1.0f);
        
        char label[128];
        snprintf(label, sizeof(label), "[Curve] %s%s", 
                 curve->getEquation().c_str(),
                 curve->isVisible() ? "" : " (hidden)");

        // Color indicator before header
        ImGui::PushStyleColor(ImGuiCol_Text, headerCol);
        ImGui::Text("|");
        ImGui::PopStyleColor();
        ImGui::SameLine();

        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(10.0f);

            // Inline equation editing
            ImGui::Text("Equation:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-1);
            if (ImGui::InputText("##eq", editBuffers[i], 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
                try {
                    scene.editCurve(curve, editBuffers[i]);
                    editErrors[i][0] = '\0';
                    logLines.push_back(std::string("[Graph] Edited curve: ") + editBuffers[i]);
                } catch (const std::exception& e) {
                    snprintf(editErrors[i], 256, "%s", e.what());
                    // Revert buffer to current equation
                    strncpy(editBuffers[i], curve->getEquation().c_str(), 255);
                }
            }
            // Update buffer if equation changed externally
            if (curve->getEquation() != editBuffers[i] && editErrors[i][0] == '\0') {
                strncpy(editBuffers[i], curve->getEquation().c_str(), 255);
            }

            // Show edit error
            if (editErrors[i][0] != '\0') {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                ImGui::TextWrapped("%s", editErrors[i]);
                ImGui::PopStyleColor();
            }

            ImGui::Dummy(ImVec2(0, 5));

            // Advanced Curve settings
            if (state->showAdvancedSettings) {

                
                // Full color picker
                float c[3] = {col.red, col.green, col.blue};
                if (ImGui::ColorEdit3("Color", c)) {
                    curve->setColor(c[0], c[1], c[2]);
                }

                // Line width
                float lw = curve->getLineWidth();
                if (ImGui::SliderFloat("Line Width", &lw, 0.5f, 10.0f)) {
                    curve->setLineWidth(lw);
                }

                // Line type
                const char* typeLabels[] = { "Straight", "Dashed", "Dotted" };
                int currentType = static_cast<int>(curve->getLineType());
                if (ImGui::Combo("Line Type", &currentType, typeLabels, 3)) {
                    curve->setLineType(static_cast<LineType>(currentType));
                }

                // Visibility
                bool vis = curve->isVisible();
                if (ImGui::Checkbox("Visible", &vis)) {
                    curve->setVisible(vis);
                }

                ImGui::Dummy(ImVec2(0, 5));

                // Duplicate button
                if (ImGui::Button("Duplicate")) {
                    int nextColorIdx = (i + 1) % 6;
                    const ImVec4& nc = curvePalette[nextColorIdx];
                    try {
                        scene.addCurve(curve->getEquation().c_str(), curve->getLineWidth(), 
                                      {nc.x, nc.y, nc.z});
                        logLines.push_back("[Graph] Duplicated curve: " + curve->getEquation());
                    } catch (...) {}
                }
                ImGui::SameLine();

                // Remove button
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
                if (ImGui::Button("Remove")) {
                    removeIndex = static_cast<int>(i);
                }
                ImGui::PopStyleColor(3);

            } else {
                // Simple Curve Settings
                
                // Color palette
                for (int ci = 0; ci < 6; ci++) {
                    ImGui::PushID(ci);
                    const ImVec4& pc = curvePalette[ci];
                    bool selected = (col.red == pc.x && col.green == pc.y && col.blue == pc.z);
                    if (selected) {
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
                    }
                    if (ImGui::ColorButton("##swatch", pc, ImGuiColorEditFlags_NoTooltip, ImVec2(36, 36))) {
                        curve->setColor(pc.x, pc.y, pc.z);
                    }
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("RGB(%.0f, %.0f, %.0f)", pc.x * 255, pc.y * 255, pc.z * 255);
                    }
                    if (selected) {
                        ImGui::PopStyleColor();
                        ImGui::PopStyleVar();
                    }
                    if (ci < 5) ImGui::SameLine();
                    ImGui::PopID();
                }

                ImGui::Dummy(ImVec2(0, 5));

                // Line type selector
                const char* typeLabels[] = { "Solid", "Dashed", "Dotted" };
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
                ImGui::Dummy(ImVec2(10, 0));
                ImGui::SameLine();

                // Visibility toggle
                bool vis = curve->isVisible();
                if (ImGui::Checkbox("Visible", &vis)) {
                    curve->setVisible(vis);
                }

                ImGui::Dummy(ImVec2(0, 5));

                // Duplicate and Remove in simple mode too
                if (ImGui::SmallButton("Duplicate")) {
                    int nextColorIdx = (i + 1) % 6;
                    const ImVec4& nc = curvePalette[nextColorIdx];
                    try {
                        scene.addCurve(curve->getEquation().c_str(), curve->getLineWidth(), 
                                      {nc.x, nc.y, nc.z});
                        logLines.push_back("[Graph] Duplicated curve: " + curve->getEquation());
                    } catch (...) {}
                }
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
                if (ImGui::SmallButton("Remove")) {
                    removeIndex = static_cast<int>(i);
                }
                ImGui::PopStyleColor(2);
            }

            ImGui::Unindent(10.0f);
        }

        ImGui::PopID();
    }

    // Deferred Removals
    // -----------------
    if (removeIndex >= 0) {
        Curve2D* curve = scene.getCurve(static_cast<size_t>(removeIndex));
        if (curve) {
            std::string eqName = curve->getEquation();  // Save before removal
            // Clean up edit buffers
            editBuffers.erase(static_cast<size_t>(removeIndex));
            editErrors.erase(static_cast<size_t>(removeIndex));
            scene.removeCurve(curve);
            logLines.push_back("[Graph] Removed curve: " + eqName);
        }
    }

    if (!removeConstant.empty()) {
        if (scene.getConstants().Exists(removeConstant)) {
            animStates.erase(removeConstant);
            scene.removeConstant(removeConstant);
            logLines.push_back("[Graph] Removed constant: " + removeConstant);
            
            // Validate all curves after constant removal
            // Curves that depend on this constant will fail to regenerate
            for (size_t i = 0; i < scene.getCurveCount(); i++) {
                Curve2D* c = scene.getCurve(i);
                if (!c) continue;
                
                // Ensure error buffer exists
                if (editErrors.find(i) == editErrors.end()) {
                    editErrors[i][0] = '\0';
                }
                
                try {
                    // Try to regenerate the curve with current constants
                    c->generate(scene.getView(), &scene.getConstants());
                    // Success. clear any previous error
                    editErrors[i][0] = '\0';
                } catch (const std::exception& e) {
                    // Curve depends on deleted constant
                    snprintf(editErrors[i], 256, "Missing constant '%s': %s", 
                             removeConstant.c_str(), e.what());
                    c->setVisible(false);
                    logLines.push_back("[Error] Curve '" + c->getEquation() + 
                                      "' disabled: depends on removed constant '" + 
                                      removeConstant + "'");
                }
            }
        } else {
            logLines.push_back("[Error] Constant not found: " + removeConstant);
        }
    }

    ImGui::End();
}
