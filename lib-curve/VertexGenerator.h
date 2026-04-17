#ifndef _VERTEX_GENERATOR_H_
#define _VERTEX_GENERATOR_H_

#include <assist.h>
#include <Expression.h>
#include <SymbolTable.h>
#include <vector>
#include <cmath>

std::vector<std::vector<float>> generateGraphPoints(
    const char* equation, 
    GraphView view,
    const SymbolTable* globalConstants = nullptr
);

#endif /* _VERTEX_GENERATOR_H_ */