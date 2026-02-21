#include "VertexGenerator.h"
#include <algorithm>
#include <limits>

static inline bool isFinite(float v) { return std::isfinite(v); }

// Push a vertex into the current strip, or start new sub-strip.
static inline void emitVertex(
    float x, float y,
    GraphView view,
    std::vector<std::vector<float>>& strips,
    bool& inStrip
) {
    if (!inStrip) {
        strips.emplace_back();    // start a new sub-strip
        inStrip = true;
    }
    auto& s = strips.back();
    s.push_back(mapToScreen(x, view.minX, view.maxX));
    s.push_back(mapToScreen(y, view.minY, view.maxY));
    s.push_back(0.0f);
}

static float computeScreenError(
    Expression& expr,
    float x1, float y1,
    float x2, float y2,
    float scaleX, float scaleY
) {
    float cdx = (x2 - x1) * scaleX;
    float cdy = (y2 - y1) * scaleY;
    float chordLen = std::sqrt(cdx * cdx + cdy * cdy);

    float maxError = 0.0f;
    for (float t : {0.25f, 0.5f, 0.75f}) {
        float xs = x1 + t * (x2 - x1);
        float ys = expr.evaluate(xs);

        if (!isFinite(ys)) return std::numeric_limits<float>::infinity();

        float pxs = (xs - x1) * scaleX;
        float pys = (ys - y1) * scaleY;

        float dist;
        if (chordLen > 1e-12f) {
            dist = std::abs(cdx * pys - cdy * pxs) / chordLen;
        } else {
            dist = std::sqrt(pxs * pxs + pys * pys);
        }
        maxError = std::max(maxError, dist);
    }
    return maxError;
}

static void adaptiveTessellate(
    Expression& expr,
    float x1, float y1,
    float x2, float y2,
    GraphView view,
    std::vector<std::vector<float>>& strips,
    bool& inStrip,
    float scaleX, float scaleY,
    float tolerance,
    int depth,
    int maxDepth
) {
    bool fin1 = isFinite(y1);
    bool fin2 = isFinite(y2);

    // TODO: Evaluation at AT points cannot yeild infinite values.
    //       Strips can't break and artificat lines occur at asymptotes.
    // Both endpoints non-finite → entire segment is outside the domain.
    if (!fin1 && !fin2) {
        inStrip = false;   // break the strip
        return;
    }

    // TODO: Same issue. Code auto skips to normal AT implementation.
    // One endpoint non-finite → domain boundary inside, subdivide to find it.
    if (!fin1 || !fin2) {
        if (depth < maxDepth) {
            float xMid = (x1 + x2) * 0.5f;
            float yMid = expr.evaluate(xMid);
            adaptiveTessellate(expr, x1, y1, xMid, yMid, view, strips, inStrip, scaleX, scaleY, tolerance, depth + 1, maxDepth);
            adaptiveTessellate(expr, xMid, yMid, x2, y2, view, strips, inStrip, scaleX, scaleY, tolerance, depth + 1, maxDepth);
        } else {
            // Max depth reached — emit whichever endpoint is finite
            if (fin1) emitVertex(x1, y1, view, strips, inStrip);
            else      inStrip = false;
        }
        return;
    }

    // Both endpoints finite → normal flatness test.
    float error = computeScreenError(expr, x1, y1, x2, y2, scaleX, scaleY);

    if (error > tolerance && depth < maxDepth) {
        // Not flat enough yet. subdivide.
        float xMid = (x1 + x2) * 0.5f;
        float yMid = expr.evaluate(xMid);
        adaptiveTessellate(expr, x1, y1, xMid, yMid, view, strips, inStrip, scaleX, scaleY, tolerance, depth + 1, maxDepth);
        adaptiveTessellate(expr, xMid, yMid, x2, y2, view, strips, inStrip, scaleX, scaleY, tolerance, depth + 1, maxDepth);
    } else if (error <= tolerance) {
        // Segment is flat enough. emit the start point.
        emitVertex(x1, y1, view, strips, inStrip);
    } else {
        inStrip = false;
    }
}


std::vector<std::vector<float>> generateGraphPoints(const char* equation, GraphView view) {
    std::vector<std::vector<float>> strips;
    Expression expr = Expression::parse(equation);
    if (!expr.isValid()) {
        throw std::runtime_error("Invalid equation: " + expr.getError());
    }

    float scaleX = 2.0f / (view.maxX - view.minX);
    float scaleY = 2.0f / (view.maxY - view.minY);

    const float tolerance = 0.001f;
    const int   maxDepth  = 12;
    const int numSegments = 64;
    float step = (view.maxX - view.minX) / numSegments;

    bool inStrip = false;

    for (int i = 0; i < numSegments; ++i) {
        float x1 = view.minX + i * step;
        float x2 = view.minX + (i + 1) * step;
        float y1 = expr.evaluate(x1);
        float y2 = expr.evaluate(x2);
        adaptiveTessellate(expr, x1, y1, x2, y2, view, strips, inStrip, scaleX, scaleY, tolerance, 0, maxDepth);
    }

    float finalX = view.maxX;
    float finalY = expr.evaluate(finalX);
    if (isFinite(finalY)) {
        emitVertex(finalX, finalY, view, strips, inStrip);
    }

    strips.erase(
        std::remove_if(strips.begin(), strips.end(),
                       [](const std::vector<float>& s) { return s.size() < 6; }),
        strips.end()
    );

    return strips;
}
