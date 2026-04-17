#include "VertexGenerator.h"
#include "SymbolTable.h"
#include <Exceptions.h>
#include <algorithm>
#include <limits>

static inline bool isFinite(float v) { return std::isfinite(v); }

// Push a vertex into the current strip, or start new sub-strip.
// xCoord and yCoord are the world-space coordinates to map
static inline void emitVertex(
    float xCoord, float yCoord,
    GraphView view,
    std::vector<std::vector<float>>& strips,
    bool& inStrip
) {
    if (!inStrip) {
        strips.emplace_back();    // start a new sub-strip
        inStrip = true;
    }
    auto& s = strips.back();
    s.push_back(mapToScreen(xCoord, view.minX, view.maxX));
    s.push_back(mapToScreen(yCoord, view.minY, view.maxY));
    s.push_back(0.0f);
}

// Compute screen-space error for adaptive tessellation
// t1, t2: the independent variable values
// d1, d2: the dependent variable values
static float computeScreenError(
    Expression& expr,
    SymbolTable& symbols,
    const std::string& independentVar,
    float t1, float d1,
    float t2, float d2,
    float scaleIndep, float scaleDep
) {
    float cdx = (t2 - t1) * scaleIndep;
    float cdy = (d2 - d1) * scaleDep;
    float chordLen = std::sqrt(cdx * cdx + cdy * cdy);

    float maxError = 0.0f;
    for (float t : {0.25f, 0.5f, 0.75f}) {
        float ts = t1 + t * (t2 - t1);
        symbols.SetValue(independentVar, ts);
        float ds = expr.evaluate(symbols);

        if (!isFinite(ds)) return std::numeric_limits<float>::infinity();

        float pxs = (ts - t1) * scaleIndep;
        float pys = (ds - d1) * scaleDep;

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

// Adaptive tessellation
static void adaptiveTessellate(
    Expression& expr,
    SymbolTable& symbols,
    const std::string& independentVar,
    bool swapAxes,  // If true, independent var is Y axis, dependent is X axis
    float t1, float d1,
    float t2, float d2,
    GraphView view,
    std::vector<std::vector<float>>& strips,
    bool& inStrip,
    float scaleIndep, float scaleDep,
    float tolerance,
    int depth,
    int maxDepth
) {
    bool fin1 = isFinite(d1);
    bool fin2 = isFinite(d2);

    // Both endpoints non-finite → entire segment is outside the domain.
    if (!fin1 && !fin2) {
        inStrip = false;   // break the strip
        return;
    }

    // One endpoint non-finite → domain boundary inside, subdivide to find it.
    if (!fin1 || !fin2) {
        if (depth < maxDepth) {
            float tMid = (t1 + t2) * 0.5f;
            symbols.SetValue(independentVar, tMid);
            float dMid = expr.evaluate(symbols);
            adaptiveTessellate(expr, symbols, independentVar, swapAxes, t1, d1, tMid, dMid, view, strips, inStrip, scaleIndep, scaleDep, tolerance, depth + 1, maxDepth);
            adaptiveTessellate(expr, symbols, independentVar, swapAxes, tMid, dMid, t2, d2, view, strips, inStrip, scaleIndep, scaleDep, tolerance, depth + 1, maxDepth);
        } else {
            // Max depth reached, emit whichever endpoint is finite
            if (fin1) {
                if (swapAxes) {
                    emitVertex(d1, t1, view, strips, inStrip);  // x = d, y = t
                } else {
                    emitVertex(t1, d1, view, strips, inStrip);  // x = t, y = d
                }
            } else {
                inStrip = false;
            }
        }
        return;
    }

    float error = computeScreenError(expr, symbols, independentVar, t1, d1, t2, d2, scaleIndep, scaleDep);

    if (error > tolerance && depth < maxDepth) {
        float tMid = (t1 + t2) * 0.5f;
        symbols.SetValue(independentVar, tMid);
        float dMid = expr.evaluate(symbols);
        adaptiveTessellate(expr, symbols, independentVar, swapAxes, t1, d1, tMid, dMid, view, strips, inStrip, scaleIndep, scaleDep, tolerance, depth + 1, maxDepth);
        adaptiveTessellate(expr, symbols, independentVar, swapAxes, tMid, dMid, t2, d2, view, strips, inStrip, scaleIndep, scaleDep, tolerance, depth + 1, maxDepth);
    } else if (error <= tolerance) {
        if (swapAxes) {
            emitVertex(d1, t1, view, strips, inStrip);  // x = d, y = t
        } else {
            emitVertex(t1, d1, view, strips, inStrip);  // x = t, y = d
        }
    } else {
        inStrip = false;
    }
}

std::vector<std::vector<float>> generateGraphPoints(
    const char* equation, 
    GraphView view,
    const SymbolTable* globalConstants
) {
    std::vector<std::vector<float>> strips;
    Expression expr = Expression::parse(equation);
    if (!expr.isValid()) {
        throw std::runtime_error("Invalid equation: " + expr.getError());
    }

    const ExpressionMetadata& meta = expr.getMetadata();
    
    // Check if the expression is drawable
    if (!meta.isDrawable) {
        if (meta.kind == ExpressionKind::ConstantAssignment) {
            // Constant assignments don't generate vertices
            return strips;
        }
        throw std::runtime_error("Expression cannot be drawn: " + expr.getError());
    }

    SymbolTable symbols;
    
    // Merge global constants if provided
    if (globalConstants) {
        symbols.MergeConstants(*globalConstants);
    }
    
    // Determine which axis to iterate
    const std::string& independentVar = meta.independentVar;
    bool swapAxes = (independentVar == "y");  // If iterating y, swap axes
    
    // Add the independent variable to the symbol table
    if (!symbols.Exists(independentVar)) {
        symbols.AddEntry(independentVar, VariableType::Coordinate);
    }
    
    // Determine range for iteration
    float minT, maxT;
    float scaleIndep, scaleDep;
    
    if (swapAxes) {
        // Iterating y, computing x
        minT = view.minY;
        maxT = view.maxY;
        scaleIndep = 2.0f / (view.maxY - view.minY);
        scaleDep = 2.0f / (view.maxX - view.minX);
    } else {
        // Iterating x, computing y
        minT = view.minX;
        maxT = view.maxX;
        scaleIndep = 2.0f / (view.maxX - view.minX);
        scaleDep = 2.0f / (view.maxY - view.minY);
    }

    const float tolerance = 0.001f;
    const int   maxDepth  = 12;
    const int numSegments = 64;
    float step = (maxT - minT) / numSegments;

    bool inStrip = false;

    for (int i = 0; i < numSegments; ++i) {
        float t1 = minT + i * step;
        float t2 = minT + (i + 1) * step;
        symbols.SetValue(independentVar, t1);
        float d1 = expr.evaluate(symbols);
        symbols.SetValue(independentVar, t2);
        float d2 = expr.evaluate(symbols);
        adaptiveTessellate(expr, symbols, independentVar, swapAxes, t1, d1, t2, d2, view, strips, inStrip, scaleIndep, scaleDep, tolerance, 0, maxDepth);
    }

    // Emit final point
    float finalT = maxT;
    symbols.SetValue(independentVar, finalT);
    float finalD = expr.evaluate(symbols);
    if (isFinite(finalD)) {
        if (swapAxes) {
            emitVertex(finalD, finalT, view, strips, inStrip);  // x = d, y = t
        } else {
            emitVertex(finalT, finalD, view, strips, inStrip);  // x = t, y = d
        }
    }

    // Remove strips with too few vertices
    strips.erase(
        std::remove_if(strips.begin(), strips.end(),
                       [](const std::vector<float>& s) { return s.size() < 6; }),
        strips.end()
    );

    return strips;
}
