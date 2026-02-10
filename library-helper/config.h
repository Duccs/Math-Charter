#ifndef CONFIG_H
#define CONFIG_H

#define ShowMessages 1  // Set to 1 to enable debug messages, 0 to disable

#include <iostream>

#if ShowMessages
    #define MSG(X) std::cout << X << std::endl;
#else
    #define MSG(X)
#endif

// Screen dimensions
constexpr unsigned int SCR_WIDTH = 1600;
constexpr unsigned int SCR_HEIGHT = 1200;

#endif // CONFIG_H
