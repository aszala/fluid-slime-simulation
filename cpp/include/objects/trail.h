#ifndef TRAIL_H
#define TRAIL_H

#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <objects/gameobject.h>

GLfloat TRAIL_DEATH_RATE = 5.0f;

class Trail : public GameObject {
    private:
        GLfloat lifespan = 255.0f;
    public:
        Trail(GLfloat spawnX, GLfloat spawnY) {
            setLocation(spawnX, spawnY);
        }

        void update() {
            this->lifespan -= TRAIL_DEATH_RATE;
        }

        GLfloat getLifeSpan() {
            return this->lifespan;
        }

};

#endif