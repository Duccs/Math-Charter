#include "AxisLabels-UI.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace {

    // Adjustable constants
    constexpr float TICK_LENGTH_PX   = 5.0f;
    constexpr float EDGE_PADDING_PX  = 4.0f;
    constexpr float LABEL_GAP_PX     = 3.0f;

    // Actual world-space bound visbile in the FBO after shader's aspect correction. 
    struct VisibleBounds {
        float minX, maxX, minY, maxY;
    };

    VisibleBounds computeVisibleBounds(const GraphView& view, ImVec2 size) {
        const float aspect = size.x / size.y;
        const float midX  = (view.minX + view.maxX) * 0.5f;
        const float midY  = (view.minY + view.maxY) * 0.5f;
        const float halfX = (view.maxX - view.minX) * 0.5f;
        const float halfY = (view.maxY - view.minY) * 0.5f;

        VisibleBounds vb;
        if (aspect >= 1.0f) {
            vb.minX = view.minX;
            vb.maxX = view.maxX;
            vb.minY = midY - halfY / aspect;
            vb.maxY = midY + halfY / aspect;
        } else {
            vb.minX = midX - halfX * aspect;
            vb.maxX = midX + halfX * aspect;
            vb.minY = view.minY;
            vb.maxY = view.maxY;
        }
        return vb;
    }

    // Linear world to pixel mapping across the visible rect.
    inline float worldToPixelX(float x, ImVec2 origin, ImVec2 size, const VisibleBounds& vb) {
        return origin.x + (x - vb.minX) / (vb.maxX - vb.minX) * size.x;
    }
    inline float worldToPixelY(float y, ImVec2 origin, ImVec2 size, const VisibleBounds& vb) {
        return origin.y + (vb.maxY - y) / (vb.maxY - vb.minY) * size.y;
    }

    void formatTickLabel(char* buf, size_t n, float value, float spacing) {
        // Snap values within 1e-4 * spacing of an integer to kill -0.0 and 1.000 noise.
        if (std::fabs(value - std::round(value)) < spacing * 1e-4f) {
            value = std::round(value);
            if (value == 0.0f) value = 0.0f; // collapse -0.0
        }

        float absSpacing = std::fabs(spacing);
        if (absSpacing >= 1e5f || (absSpacing > 0.0f && absSpacing < 1e-3f)) {
            std::snprintf(buf, n, "%g", value);
            return;
        }

        int decimals = static_cast<int>(std::max(0.0f, -std::floor(std::log10(absSpacing))));
        if (decimals > 6) decimals = 6;
        std::snprintf(buf, n, "%.*f", decimals, value);
    }

    void drawXAxisTicks(ImDrawList* dl, ImVec2 origin, ImVec2 size,
                        const VisibleBounds& vb, float spacing, ImU32 col) {
        const bool axisVisible = (vb.minY <= 0.0f && vb.maxY >= 0.0f);
        // When the axis is off-screen, pin to the edge.
        const bool pinBottom = !axisVisible && vb.minY > 0.0f;
        float axisPy;
        if (axisVisible)       axisPy = worldToPixelY(0.0f, origin, size, vb);
        else if (pinBottom)    axisPy = origin.y + size.y - EDGE_PADDING_PX;
        else                   axisPy = origin.y + EDGE_PADDING_PX;

        const float start = std::floor(vb.minX / spacing) * spacing;
        char buf[32];

        for (float x = start; x <= vb.maxX + spacing * 0.5f; x += spacing) {
            if (std::fabs(x) < spacing * 1e-4f) continue; // skip origin
            const float px = worldToPixelX(x, origin, size, vb);
            if (px < origin.x || px > origin.x + size.x) continue;

            dl->AddLine(ImVec2(px, axisPy - TICK_LENGTH_PX),
                        ImVec2(px, axisPy + TICK_LENGTH_PX), col, 1.0f);

            formatTickLabel(buf, sizeof(buf), x, spacing);
            const ImVec2 ts = ImGui::CalcTextSize(buf);
            // Labels sit inward relative to the edge or below the axis when visible.
            const bool labelAbove = !axisVisible && pinBottom;
            const float ty = labelAbove
                ? axisPy - TICK_LENGTH_PX - LABEL_GAP_PX - ts.y
                : axisPy + TICK_LENGTH_PX + LABEL_GAP_PX;
            dl->AddText(ImVec2(px - ts.x * 0.5f, ty), col, buf);
        }
    }

    void drawYAxisTicks(ImDrawList* dl, ImVec2 origin, ImVec2 size,
                        const VisibleBounds& vb, float spacing, ImU32 col) {
        const bool axisVisible = (vb.minX <= 0.0f && vb.maxX >= 0.0f);
        // When the axis is off-screen, pin to the edge.
        const bool pinLeft = !axisVisible && vb.minX > 0.0f;
        float axisPx;
        if (axisVisible)       axisPx = worldToPixelX(0.0f, origin, size, vb);
        else if (pinLeft)      axisPx = origin.x + EDGE_PADDING_PX;
        else                   axisPx = origin.x + size.x - EDGE_PADDING_PX;

        const float start = std::floor(vb.minY / spacing) * spacing;
        char buf[32];

        for (float y = start; y <= vb.maxY + spacing * 0.5f; y += spacing) {
            if (std::fabs(y) < spacing * 1e-4f) continue;
            const float py = worldToPixelY(y, origin, size, vb);
            if (py < origin.y || py > origin.y + size.y) continue;

            dl->AddLine(ImVec2(axisPx - TICK_LENGTH_PX, py),
                        ImVec2(axisPx + TICK_LENGTH_PX, py), col, 1.0f);

            formatTickLabel(buf, sizeof(buf), y, spacing);
            const ImVec2 ts = ImGui::CalcTextSize(buf);
            // Labels sit inward relative to the edge or left of the axis when visible.
            const bool labelLeft = !axisVisible && !pinLeft;
            const float tx = labelLeft
                ? axisPx - TICK_LENGTH_PX - LABEL_GAP_PX - ts.x
                : axisPx + TICK_LENGTH_PX + LABEL_GAP_PX;
            dl->AddText(ImVec2(tx, py - ts.y * 0.5f), col, buf);
        }
    }

} // namespace

void DrawAxisTickOverlay(ImDrawList* drawList,
                         ImVec2 origin, ImVec2 size,
                         const GraphView& view,
                         float spacing) {
    if (!drawList || size.x <= 0.0f || size.y <= 0.0f) return;
    if (!(spacing > 0.0f) || !std::isfinite(spacing)) return;
    if (view.maxX <= view.minX || view.maxY <= view.minY) return;

    const VisibleBounds vb = computeVisibleBounds(view, size);
    const ImU32 col = ImGui::GetColorU32(ImGuiCol_Text);

    drawList->PushClipRect(origin, ImVec2(origin.x + size.x, origin.y + size.y), true);

    drawXAxisTicks(drawList, origin, size, vb, spacing, col);
    drawYAxisTicks(drawList, origin, size, vb, spacing, col);

    // Origin 0 label at axis intersection, only when the origin is actually visible.
    if (vb.minX <= 0.0f && vb.maxX >= 0.0f &&
        vb.minY <= 0.0f && vb.maxY >= 0.0f) {
        const float ox = worldToPixelX(0.0f, origin, size, vb);
        const float oy = worldToPixelY(0.0f, origin, size, vb);
        drawList->AddText(ImVec2(ox + TICK_LENGTH_PX + LABEL_GAP_PX,
                                 oy + TICK_LENGTH_PX + LABEL_GAP_PX),
                          col, "0");
    }

    drawList->PopClipRect();
}
