#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

#include <utils/geom.h>
#include <utils/vector2.h>
#include <objects/gameobject.h>
#include <shaders/shader.h>

// #include <objects/trail.h>
// #include <objects/agent.h>

// void update(Agent* agent) {
//     GLfloat angle = agent->getAngle();

//     GLfloat deltaX = sin(angle) * AGENT_SPEED;
//     GLfloat deltaY = -cos(angle) * AGENT_SPEED;

//     GLfloat newX = agent->predictX(deltaX);
//     GLfloat newY = agent->predictY(deltaY);

//     if (newX < 0 || newX > SCREEN_WIDTH-10 || newY < 0 || newY > SCREEN_HEIGHT-10) {
//         newX = std::min(SCREEN_WIDTH-10.0f, std::max(0.0f, newX));
//         newY = std::min(SCREEN_HEIGHT-10.0f, std::max(0.0f, newY));

//         agent->setAngle(generateRandomAngle());
//     }

//     agent->moveTo(newX, newY);

//     std::vector<Trail*>* trails = agent->getTrail();

//     unsigned int i=0;
//     for (i=0; i<trails->size(); i++) {
//         Trail* trail = (*trails)[i];
//         trail->update();

//         if (trail->getLifeSpan() < 5) {
//             trails->erase(trails->begin());
//         }
//     }
// }

// void draw(Agent* agent) {
//     //drawCircle(agent->getX(), agent->getY(), 0, AGENT_WIDTH, 4);
//     drawRectangle(agent->getX(), agent->getY(), AGENT_WIDTH, 255.0f);

//     std::vector<Trail*>* trails = agent->getTrail();

//     unsigned int i=0;
//     for (i=0; i<trails->size(); i++) {
//         Trail* trail = (*trails)[i];

//         drawRectangle(trail->getX(), trail->getY(), AGENT_WIDTH, trail->getLifeSpan());
//     }
// }
void resizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(void) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Slime Simulations", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glewInit();


	Shader vertexShader;
	vertexShader.InitFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
	Shader fragmentShader;
	fragmentShader.InitFromFile("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();

	vertexShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);
	glLinkProgram(shaderProgram);

	Shader computeShader;
	computeShader.InitFromFile("shaders/compute.glsl", GL_COMPUTE_SHADER);
    GLuint computeProgram = glCreateProgram();
    computeShader.AttachTo(computeProgram);
    glLinkProgram(computeProgram);

    // std::vector<Agent*> agents;
    
    GLint sleepTime = 1000/60;

    // for (i=0;i<1000;i++) {
    //     Agent* agent = new Agent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        
    //     GLfloat angle = generateRandomAngle();
    //     agent->setAngle(angle);

    //     agents.push_back(agent);
    // }

	
	GameObject* gameObject = new GameObject(SCREEN_WIDTH/2, SCREEN_WIDTH/3);

    while (!glfwWindowShouldClose(window)) {
        //auto start = std::chrono::high_resolution_clock::now();

		float dt = glfwGetTime();
        glfwSetTime(0);

        glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gameObject->m_vertexBuffer);
        glUseProgram(computeProgram);
        glUniform1f(0, dt);
        glDispatchCompute(gameObject->vertices.size(), 1, 1);

		glUseProgram(shaderProgram);
		gameObject->Draw();
		glUseProgram(0);

        // for (i=0; i<agents.size(); i++) {
        //     Agent* agent = agents[i];

        //     update(agent);
        //     draw(agent);
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();

        // auto finish = std::chrono::high_resolution_clock::now();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ms\n";
    }

	delete gameObject;
    
    glfwTerminate();

    // for (i=0; i<agents.size(); i++) {
    //     std::vector<Trail*>* trails = agents[i]->getTrail();

    //     unsigned int j=0;
    //     for (j=0; j<trails->size(); j++) {
    //         delete (*trails)[j];
    //     }

    //     delete agents[i];
    // }
    
    return 0;
}