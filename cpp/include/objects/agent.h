#ifndef AGENT_H
#define AGENT_H

#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Agent : public GameObject {
    public:
        Agent(GLfloat spawnX, GLfloat spawnY) {
            setLocation(spawnX, spawnY);
        }

        void moveTo(GLfloat newX, GLfloat newY) {
            setLocation(newX, newY);
        }

};

#endif