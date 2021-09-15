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
#include <shaders/shader.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void update(Agent* agent) {
    GLfloat angle = agent->getAngle();

    GLfloat deltaX = sin(angle) * AGENT_SPEED;
    GLfloat deltaY = -cos(angle) * AGENT_SPEED;

    GLfloat newX = agent->predictX(deltaX);
    GLfloat newY = agent->predictY(deltaY);

    if (newX < 0 || newX > SCREEN_WIDTH-10 || newY < 0 || newY > SCREEN_HEIGHT-10) {
        newX = std::min(SCREEN_WIDTH-10.0f, std::max(0.0f, newX));
        newY = std::min(SCREEN_HEIGHT-10.0f, std::max(0.0f, newY));

        agent->setAngle(generateRandomAngle());
    }

    agent->moveTo(newX, newY);
}

void draw(Agent* agent) {
    //drawCircle(agent->getX(), agent->getY(), 0, AGENT_WIDTH, 4);
    drawRectangle(agent->getX(), agent->getY(), AGENT_WIDTH);
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

    //Shader shader("../shaders/compute.es");

    // const char *source;
    // int length;

    // std::string integrate_source =
    //     "#version 430\n"
    //     "layout(local_size_x=256) in;\n"

    //     "layout(location = 0) uniform float dt;\n"
    //     "layout(std430, binding=0) buffer pblock { vec4 positions[]; };\n"
    //     "layout(std430, binding=1) buffer vblock { vec4 velocities[]; };\n"

    //     "void main() {\n"
    //     "   int index = int(gl_GlobalInvocationID);\n"
    //     "   vec4 position = positions[index];\n"
    //     "   vec4 velocity = velocities[index];\n"
    //     "   position.xyz += dt*velocity.xyz;\n"
    //     "   positions[index] = position;\n"
    //     "}\n";


    // GLenum err = glewInit();

    // GLuint integrate_program, integrate_shader;

    // integrate_shader = glCreateShader(GL_COMPUTE_SHADER);
    // source = integrate_source.c_str();
    // length = integrate_source.size();
    // glShaderSource(integrate_shader, 1, &source, &length);
    // glCompileShader(integrate_shader);

    // integrate_program = glCreateProgram();

    // glAttachShader(integrate_program, integrate_shader);

    // glLinkProgram(integrate_program);
    




    unsigned int i = 0;

    std::vector<Agent*> agents;
    
    GLint sleepTime = 1000/60;

    for (i=0;i<10000;i++) {
        Agent* agent = new Agent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        
        GLfloat angle = generateRandomAngle();
        agent->setAngle(angle);

        agents.push_back(agent);
    }

    while (!glfwWindowShouldClose(window)) {
        //auto start = std::chrono::high_resolution_clock::now();
        glClear(GL_COLOR_BUFFER_BIT);

        for (i=0; i<agents.size(); i++) {
            Agent* agent = agents[i];

            update(agent);
            draw(agent);
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();

        // auto finish = std::chrono::high_resolution_clock::now();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ms\n";
    }
    
    for (i=0; i<agents.size(); i++) {
        delete agents[i];
    }

    glfwTerminate();
    
    return 0;
}