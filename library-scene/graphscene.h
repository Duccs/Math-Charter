#ifndef _GRAPHSCENE_H_
#define _GRAPHSCENE_H_

#include <curve2d.h>
#include <shader.h>
#include <vector>
#include <memory>

class GraphScene { 
    private:
        GraphView view;
        std::vector<std::unique_ptr<Curve2D>> curves;

        // Grid resources
        std::vector<float> gridLines;
        unsigned int gridVAO, gridVBO;
        float gridSpacing;

    public:
        GraphScene(GraphView initialView, float gridSpacing = 1.0f);
        ~GraphScene();

        Curve2D* addCurve(const char* equation, int numPoints);
        void removeCurve(Curve2D* curve);

        void updateView(GraphView newView);
        void pan(float dx, float dy);
        void zoom(float factor);

        void render(Shader& shader, float aspectRatio);
        void cleanup();

        GraphView& getView() { return view; }
        const GraphView& getView() const { return view; }

};

#endif /* _GRAPHSCENE_H_ */