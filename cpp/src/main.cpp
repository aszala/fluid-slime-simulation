#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

#include <geom/geom.h>
#include <geom/vector2.h>

#include <objects/gameobject.h>
#include <objects/agent.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void update(Agent* agent) {
    GLfloat angle = agent->getAngle();

    GLfloat deltaX = sin(angle) * AGENT_SPEED;
    GLfloat deltaY = -cos(angle) * AGENT_SPEED;

    GLfloat newX = agent->predictX(deltaX);
    GLfloat newY = agent->predictX(deltaY);

    agent->moveTo(newX, newY);
}

void draw(Agent* agent) {
    drawCircle(agent->getX(), agent->getY(), 0, AGENT_WIDTH, 4);
}

int main(void) {
    GLFWwindow *window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Slime Simulation", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /// Start

    std::vector<Agent*> agents;
    
    GLint sleepTime = 1000/60;

    for (unsigned int i=0;i<10;i++) {
        Agent agent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        GLfloat angle = generateRandomAngle();

        agent.setAngle(angle);
        agents.push_back(&agent);
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        auto iter = agents.begin();

        for (; iter != agents.end(); iter++) {
            Agent* agent = (*iter);

            update(agent);
            draw(agent);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}