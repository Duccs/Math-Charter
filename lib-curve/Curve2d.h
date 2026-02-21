#ifndef _CURVE2D_H_
#define _CURVE2D_H_

#include "Line2d.h"
#include <string>


class Curve2D : public Line2D
{
    protected:
        std::string equation;

    public:
        Curve2D(const char* equation, float lineWidth = 2.0f, RenderColor color = {0.0f, 0.0f, 0.0f});
        ~Curve2D() override;

        void generate(GraphView view) override;  // Generate vertex data

        void setEquation(const char* equation);
        const std::string& getEquation() const;
};


#endif /* _CURVE2D_H_ */