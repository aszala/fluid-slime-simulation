#ifndef AGENT_H
#define AGENT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <objects/gameobject.h>

GLfloat AGENT_SPEED = 1.0f;
GLfloat AGENT_WIDTH = 2.0f;
GLfloat SENSOR_ANGLE = -0.785398; // -45 as radian

GLfloat LOW_ANGLE = -180.0f;
GLfloat HIGH_ANGLE = 360.0f;

GLfloat generateRandomAngle() {
    GLfloat r3 = LOW_ANGLE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HIGH_ANGLE-LOW_ANGLE)));

    return r3;
}

class Agent : public GameObject {
    private:
        int adwjkjdlk;
    public:
        Agent(GLfloat spawnX, GLfloat spawnY) {
            setLocation(spawnX, spawnY);
        }

        void moveTo(GLfloat newX, GLfloat newY) {
            setLocation(newX, newY);
            // Add trail map
        }

};

#endif