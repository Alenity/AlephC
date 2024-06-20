#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>


struct widgetProps {
    glm::vec4 padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 margin = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    int colourLoc = 0;
    int index = 0;
};


#endif