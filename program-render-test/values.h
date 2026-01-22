    #ifndef _RENDER_VALUES_H_
    #define _RENDER_VALUES_H_
    
    // Configure vertices for drawing shapes
    float vertices[] = {
        // Positions         // Colors          // Texture Coords
         0.0f, -0.43f, 0.0f,  1.0f, 0.0f, 0.0f,  0.5f, 0.0f,   // Middle Ground 0
         0.25f, 0.0f,  0.0f,  0.0f, 1.0f, 0.0f,  0.75f, 0.5f,  // Right Top     1
         0.5f, -0.43f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,   // Right Corner  2
        -0.25f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,  0.25f, 0.5f,  // Left Top      3
        -0.5f, -0.43f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // Left Corner   4
         0.0f,  0.43f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f    // Middle Peak   5
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 3, 4,   // first Triangle
        0, 1, 2,   // second Triangle
        1, 3, 5    // third Triangle
    };
    float texCoords[] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };

    #endif /* _RENDER_VALUES_H_ */