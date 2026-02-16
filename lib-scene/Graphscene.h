#ifndef _GRAPHSCENE_H_
#define _GRAPHSCENE_H_

#include <Curve2d.h>
#include <Shader.h>
#include <vector>
#include <memory>

struct GridConfig {
    float spacing;
    bool skipAxis;          // Skip x=0 and y=0
    float skipTolerance;
    float skipMajorSpacing; // If > 0, also skip lines at major intervals (for minor lines)
};

class GraphScene { 
    private:
        GraphView view;
        std::vector<std::unique_ptr<Curve2D>> curves;

        // Grid resources
        std::vector<float> axisGridLines;
        std::vector<float> majorGridLines;
        std::vector<float> minorGridLines;
        unsigned int axisVAO, axisVBO;
        unsigned int majorGridVAO, majorGridVBO;
        unsigned int minorGridVAO, minorGridVBO;
        float gridSpacing;

        // Internal methods
        void initVAOnVBO(unsigned int& VAO, unsigned int& VBO);
        
        // Grid Generation methods
        float calculateAdaptiveSpacing(float viewRange);
        std::vector<float> generateGridLines(GraphView view, GridConfig config);
        std::vector<float> generateAxisLines(GraphView view);
        std::vector<float> generateMajorLines(GraphView view, float spacing);
        std::vector<float> generateMinorLines(GraphView view, float spacing);

        // For use in public GraphScene::render()
        void renderGrid(Shader& shader);

    public:
        GraphScene(GraphView initialView);
        ~GraphScene();

        // Add or remove curves
        Curve2D* addCurve(const char* equation, int numPoints, float lineWidth = 2.0f, RenderColor color = {0.0f, 0.0f, 0.0f});
        void removeCurve(Curve2D* curve);

        // View manipulation
        void updateView(GraphView newView);
        void pan(float dx, float dy);
        void zoom(float factor);
        void zoomAt(float worldX, float worldY, float factor);
        void render(Shader& shader, float aspectRatio);

        // Cleanup
        void cleanup();

        // Stuff
        GraphView& getView() { return view; }
        const GraphView& getView() const { return view; }

};

#endif /* _GRAPHSCENE_H_ */