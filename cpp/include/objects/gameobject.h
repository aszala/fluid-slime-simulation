#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <vector>
#include <utils/utils.h>

class GameObject {

public:
	GameObject(GLfloat x, GLfloat y);
	~GameObject();

	void Draw();

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_vao;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

    GLfloat angle;
};

#endif