#ifndef _VERTEX_GENERATOR_H_
#define _VERTEX_GENERATOR_H_

#include <assist.h>
#include <Expression.h>
#include <vector>
#include <cmath>

std::vector<std::vector<float>> generateGraphPoints(const char* equation, GraphView view);

#endif /* _VERTEX_GENERATOR_H_ */