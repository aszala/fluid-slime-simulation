#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WIDTH 2
#define HEIGHT 2

struct Vertex {
	glm::vec4 position;
	glm::vec4 color;

	Vertex(glm::vec4 p, glm::vec4 c) {
		position = p;
		color = c;
	}
};

GLfloat mapScreenValuesFromNormalToGPU(GLfloat number, GLfloat inMax);

#endif