#ifndef _CURVE2D_H_
#define _CURVE2D_H_

#include <glad/glad.h>
#include <helper.h>

#include <string>
#include <vector>

struct RenderColor {
    float red = 0.0f;
    float green =  0.0f;
    float blue = 0.0f;
};

class Curve2D
{
    private:
        std::string equation;
        std::vector<float> points;
        unsigned int VAO, VBO;
        int numPoints;
        RenderColor color;
    public:
        Curve2D(const char* equation, int points);
        ~Curve2D();

        void generate(GraphView view);  // Generate vertex data
        void upload();                  // Upload to GPU
        void render();                  // Draw the curve
        void update(GraphView view);    // Regenerate and reupload

        void setEquation(const char* equation);
        const std::string& getEquation() const;
        int getNumPoints() const;
        void setColor(float r, float g, float b);
        RenderColor getColor() const;



};


#endif /* _CURVE2D_H_ */