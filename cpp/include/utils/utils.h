#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#define SCREEN_WIDTH 1380
#define SCREEN_HEIGHT 1380
// #define WIDTH 2
// #define HEIGHT 2

// GLfloat AGENT_SPEED = 1.0f;
// GLfloat AGENT_WIDTH = 2.0f;
// GLfloat SENSOR_ANGLE = -0.785398; // -45 as radian

#define LOW_ANGLE 0.0f
#define HIGH_ANGLE 360.0f

#define M_PI 3.14159265358979323846

// #define HEATMAP_R 0
// #define HEATMAP_G 1
// #define HEATMAP_B 1

struct Vertex {
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 angle;

	Vertex(glm::vec4 p, glm::vec4 c, glm::vec4 a) {
		position = p;
		color = c;
		angle = a;
	}
};

GLfloat mapScreenValuesFromNormalToGPU(GLfloat number, GLfloat inMax);
Vertex makeVertex(GLfloat x, GLfloat y, GLfloat angle);
GLfloat generateRandomAngle();
GLfloat* generateRGBScreenHeatMap(float* heatmap, GLfloat HEATMAP_R, GLfloat HEATMAP_G, GLfloat HEATMAP_B);

GLfloat uniform();
glm::vec2 randPointInCircle(GLfloat center_x, GLfloat center_y, GLfloat radius);
GLfloat angleToCenter(GLfloat y, GLfloat x, GLfloat center_x, GLfloat center_y);

void glfwSetWindowCenter(GLFWwindow* window);
void HSVtoRGB(GLfloat H, GLfloat S, GLfloat V, GLfloat* R, GLfloat* G, GLfloat* B);

#endif