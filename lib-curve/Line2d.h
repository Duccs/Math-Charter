#ifndef _LINE2D_H_
#define _LINE2D_H_

#include "VertexGenerator.h"
#include <glad/glad.h>

enum class LineType {
    Straight,
    Dashed,
    Dotted
};

class Line2D {

    protected:
        std::vector<float> points;
        unsigned int VAO, VBO;
        int numPoints;
        float lineWidth;
        RenderColor color;
        LineType lineType = LineType::Straight;
        bool visible = true;

    public:
        Line2D(int points, float lineWidth = 1.0f, RenderColor color = {0.0f, 0.0f, 0.0f});
        virtual ~Line2D();

        virtual void generate(GraphView view);  // Generate vertex data
        void upload();                          // Upload to GPU
        void render();                          // Draw the line/curve
        void update(GraphView view);            // Regenerate and reupload

        int getNumPoints() const;
        void setNumPoints(int points);
        void setColor(float r, float g, float b);
        RenderColor getColor() const;
        float getLineWidth() const;
        void setLineWidth(float width);

        LineType getLineType() const;
        void setLineType(LineType type);
        bool isVisible() const;
        void setVisible(bool v);
};

#endif /* _LINE2D_H_ */