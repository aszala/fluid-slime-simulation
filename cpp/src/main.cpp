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

#define SLEEP_TIME 17
#define AGENT_COUNT 1000000

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
	vertexShader.InitFromFile("src/shaders/vertex.glsl", GL_VERTEX_SHADER);
	Shader geometryShader;
	geometryShader.InitFromFile("src/shaders/geometry.glsl", GL_GEOMETRY_SHADER);
	Shader fragmentShader;
	fragmentShader.InitFromFile("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();

	vertexShader.AttachTo(shaderProgram);
    geometryShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);
	glLinkProgram(shaderProgram);

	Shader computeShader;
	computeShader.InitFromFile("src/shaders/compute.glsl", GL_COMPUTE_SHADER);
    GLuint computeProgram = glCreateProgram();
    computeShader.AttachTo(computeProgram);
    glLinkProgram(computeProgram);

    unsigned int i = 0;

    std::vector<Vertex> allVertices;

    GLfloat x = SCREEN_WIDTH / 2;
    GLfloat y = SCREEN_HEIGHT / 2;

    for (i=0; i<AGENT_COUNT; i++) {
        GLfloat startAngle = generateRandomAngle();

        allVertices.push_back(makeVertex(x, y, startAngle));
    }

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vertex), &allVertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(Vertex), (void*)(2 * sizeof(Vertex)));
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        //auto start = std::chrono::high_resolution_clock::now();

		float dt = glfwGetTime();
        glfwSetTime(0);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO);
        glUseProgram(computeProgram);
        glUniform1f(0, dt);
        glDispatchCompute(allVertices.size(), 1, 1);

		glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, allVertices.size());
        
        // std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();

        // auto finish = std::chrono::high_resolution_clock::now();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ms\n";
    }

    glfwTerminate();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}