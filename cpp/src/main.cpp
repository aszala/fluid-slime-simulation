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
#define AGENT_COUNT 100000

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
    //geometryShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);
	glLinkProgram(shaderProgram);

	Shader computeShader;
	computeShader.InitFromFile("src/shaders/compute.glsl", GL_COMPUTE_SHADER);
    GLuint computeProgram = glCreateProgram();
    computeShader.AttachTo(computeProgram);
    glLinkProgram(computeProgram);

    unsigned int i = 0;

    std::vector<Vertex> allVertices;
    std::vector<unsigned int> allIndices;

    GLfloat x = SCREEN_WIDTH / 2;
    GLfloat y = SCREEN_HEIGHT / 2;

    for (i=0; i<AGENT_COUNT; i++) {
        GLfloat startAngle = generateRandomAngle();

        allVertices.push_back(makeVertex(x, y, startAngle));
        allVertices.push_back(makeVertex(x + WIDTH, y, startAngle));
        allVertices.push_back(makeVertex(x, y + HEIGHT, startAngle));
        allVertices.push_back(makeVertex(x + WIDTH, y + HEIGHT, startAngle));

        allIndices.push_back(0 + (i * 4));
        allIndices.push_back(1 + (i * 4));
        allIndices.push_back(2 + (i * 4));
        allIndices.push_back(3 + (i * 4));
        allIndices.push_back(2 + (i * 4));
        allIndices.push_back(1 + (i * 4));
    }

    GLuint m_allVertexBuffer;
    GLuint m_allIndicesBuffer;

    glGenBuffers(1, &m_allVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_allVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vertex), &allVertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_allIndicesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_allIndicesBuffer);
	glBufferData(GL_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int), &allIndices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        //auto start = std::chrono::high_resolution_clock::now();

		float dt = glfwGetTime();
        glfwSetTime(0);

        glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_allVertexBuffer);
        glUseProgram(computeProgram);
        glUniform1f(0, dt);
        glDispatchCompute(allVertices.size(), 1, 1);

		glUseProgram(shaderProgram);

		glBindBuffer(GL_ARRAY_BUFFER, m_allVertexBuffer);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_allIndicesBuffer);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, allIndices.size(), GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();

        // auto finish = std::chrono::high_resolution_clock::now();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ms\n";
    }

    glfwTerminate();

	glDeleteBuffers(1, &m_allVertexBuffer);
	glDeleteBuffers(1, &m_allIndicesBuffer);

    return 0;
}