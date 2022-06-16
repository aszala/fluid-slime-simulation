#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

#include <utils/utils.h>
#include <shaders/shader.h>

#define AGENT_COUNT 500000
#define TRAIL_DECAY_RATE 0.1f
 
GLboolean pause = false;

void resizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pause = !pause;
    }
}

int main(void) {
    // Init window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Slime Simulations", nullptr, nullptr);
	glfwSetWindowCenter(window);
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, key_callback);
	glewInit();

    // Init Shaders
    
	GLuint heatmapShaderProgram = glCreateProgram();
	GLuint shaderProgram = glCreateProgram();
    GLuint computeProgram = glCreateProgram();

    // Particle Shader
	Shader vertexShader;
	Shader geometryShader;
	Shader fragmentShader;
    
	vertexShader.InitFromFile("src/shaders/vertex.glsl", GL_VERTEX_SHADER);
	geometryShader.InitFromFile("src/shaders/geometry.glsl", GL_GEOMETRY_SHADER);
	fragmentShader.InitFromFile("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);

	vertexShader.AttachTo(shaderProgram);
    geometryShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);


    // Particle Compute Shader
	Shader computeShader;

	computeShader.InitFromFile("src/shaders/compute.glsl", GL_COMPUTE_SHADER);
    computeShader.AttachTo(computeProgram);

    // Heatmap Shader
    Shader heatmapVertexShader;
	Shader heatmapFragmentShader;

	heatmapVertexShader.InitFromFile("src/shaders/heatmap_vertex.glsl", GL_VERTEX_SHADER);
	heatmapFragmentShader.InitFromFile("src/shaders/heatmap_fragment.glsl", GL_FRAGMENT_SHADER);

	heatmapVertexShader.AttachTo(heatmapShaderProgram);
    heatmapFragmentShader.AttachTo(heatmapShaderProgram);

    
	glLinkProgram(shaderProgram);
    glLinkProgram(computeProgram);
	glLinkProgram(heatmapShaderProgram);


    // Init Particle Agents
    unsigned int i = 0;

    std::vector<Vertex> agents;

    GLfloat center_x = SCREEN_WIDTH / 2;
    GLfloat center_y = SCREEN_HEIGHT / 2;

    for (i=0; i<AGENT_COUNT; i++) {
        // glm::vec2 startPos = randPointInCircle(center_x, center_y, 300);
        // GLfloat startAngle = angleToCenter(startPos.y, startPos.x, center_x, center_y);

        glm::vec2 startPos = glm::vec2(center_x, center_y);
        GLfloat startAngle = generateRandomAngle();

        agents.push_back(makeVertex(startPos.x, startPos.y, startAngle));
    }

    // Init heatmap
    float screenSizeBytes = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(float);
    float* screenHeatmap = (float*)malloc(screenSizeBytes);

    for (i=0;i<SCREEN_WIDTH * SCREEN_HEIGHT;i++) {
        screenHeatmap[i] = 0;
    }

    // Create VBO / VAO for the particle shaders
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
        // Store the Agents into the VBO on GPU
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, AGENT_COUNT * sizeof(Vertex), &agents[0], GL_STREAM_DRAW);
        
        // Enable VAO Locations to be passed into shaders
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(Vertex), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(Vertex), (void*)(2 * sizeof(Vertex)));

    glBindVertexArray(0);

    // Send the agent VBO into storage to be accessed by the compute shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, VBO);

    /// Pass the Heatmap into the GPU so can be accessed by the Compute shader
    unsigned int screenMapSSBO;
    glGenBuffers(1, &screenMapSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, screenMapSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, screenSizeBytes, &screenHeatmap[0], GL_STREAM_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, screenMapSSBO);

    // float agentPosByteSize = AGENT_COUNT * 2 * sizeof(float);

    // float agentPos[AGENT_COUNT * 2];
    
    // unsigned int agentPosSSBO;
    // glGenBuffers(1, &agentPosSSBO);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, agentPosSSBO);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, agentPosByteSize, &agentPos[0], GL_STREAM_COPY);
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, agentPosSSBO);
    

    // Define the heatmap mapping
    float heatmapVertices[] = {
        // positions          // colors           // texture coords
        1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 
    
    // Create VBO / VAO / EBO for the heatmap
    unsigned int VBO_HEAT, VAO_HEAT;
    unsigned int EBO_HEAT;
    glGenBuffers(1, &EBO_HEAT);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO_HEAT);

    // Store heatmap mapping into VBO / VAO to send to GPU
    glBindVertexArray(VAO_HEAT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(heatmapVertices), heatmapVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_HEAT);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Enable VAO Locations to be passed into shaders
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);


    // Create a texture for the heatmap to be stored in
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // // Texture parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    

    GLint screenWidthComputeUniform = glGetUniformLocation(computeProgram, "screen_width");
    GLint screenHeightComputeUniform = glGetUniformLocation(computeProgram, "screen_height");    
    GLint agentCountComputeUniform = glGetUniformLocation(computeProgram, "agent_count");    

    GLint screenWidthHeatmapFragment = glGetUniformLocation(heatmapShaderProgram, "heatmap_resolution_x");
    GLint screenHeightHeatmapFragment = glGetUniformLocation(heatmapShaderProgram, "heatmap_resolution_y");

    GLint screenWidthGeo = glGetUniformLocation(shaderProgram, "screen_width");
    GLint screenHeightGeo = glGetUniformLocation(shaderProgram, "screen_height");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        if (pause) {
            continue;
        }

        //auto start = std::chrono::high_resolution_clock::now();

		float dt = glfwGetTime();
        glfwSetTime(0);
        
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);

        // Run compute shader for each particle and heatmap
        glUseProgram(computeProgram);
        glUniform1f(0, dt);
        glUniform1f(1, pause);
        glUniform1i(screenWidthComputeUniform, SCREEN_WIDTH);
        glUniform1i(screenHeightComputeUniform, SCREEN_HEIGHT);
        glUniform1i(agentCountComputeUniform, AGENT_COUNT);
        glDispatchCompute(AGENT_COUNT, 1, 1);

        // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, agentPosSSBO);
        // glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, agentPosByteSize, agentPos);
        
        // for (i=0;i<AGENT_COUNT;i++) {
        //     std::cout << agentPos[i] << " , " << agentPos[i + AGENT_COUNT] << std::endl;
        //     std::cout << "----" << std::endl;
        // }

        // Extract heatmap data back from compute shader
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, screenMapSSBO);
        glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, screenSizeBytes, screenHeatmap);
        
        // Decrement heatmap density each frame
        for (i=0;i<SCREEN_WIDTH * SCREEN_HEIGHT;i++) {
            if (screenHeatmap[i] > 0) {
                screenHeatmap[i] -= TRAIL_DECAY_RATE;
            } else {
                screenHeatmap[i] = 0;
            }
        }
        
        glBufferData(GL_SHADER_STORAGE_BUFFER, screenSizeBytes, &screenHeatmap[0], GL_STREAM_COPY);

        // Render the heatmap
		glUseProgram(heatmapShaderProgram);
        
        glUniform1f(screenWidthHeatmapFragment, SCREEN_WIDTH);
        glUniform1f(screenHeightHeatmapFragment, SCREEN_HEIGHT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT,
                                                            generateRGBScreenHeatMap(screenHeatmap)); // Generate RGB from density
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Enable heatmap VAO for rendering and render the heatmap
        glBindVertexArray(VAO_HEAT);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render the particles from particle VAO
		glUseProgram(shaderProgram);
        glUniform1f(screenWidthGeo, SCREEN_WIDTH);
        glUniform1f(screenHeightGeo, SCREEN_HEIGHT);

        glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, AGENT_COUNT);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);

        // auto finish = std::chrono::high_resolution_clock::now();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ms\n";
    }

    delete screenHeatmap;

    glDeleteProgram(computeProgram);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(heatmapShaderProgram);

    glfwTerminate();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    return 0;
}