#include <objects/gameobject.h>

GameObject::GameObject(GLfloat number, GLfloat inMax) {
	vertices.push_back(
			Vertex(
				glm::vec4(mapScreenValuesFromNormalToGPU(SCREEN_WIDTH/2, SCREEN_WIDTH), mapScreenValuesFromNormalToGPU(SCREEN_HEIGHT/2, SCREEN_HEIGHT), 0, 1),
				glm::vec4(.5, .5, .5, 1)
				)
			);
	vertices.push_back(
			Vertex(
				glm::vec4(mapScreenValuesFromNormalToGPU(SCREEN_WIDTH/2 + WIDTH, SCREEN_WIDTH), mapScreenValuesFromNormalToGPU(SCREEN_HEIGHT/2, SCREEN_HEIGHT), 0, 1),
				glm::vec4(.5, .5, .5, 1)
				)
			);
	vertices.push_back(
			Vertex(
				glm::vec4(mapScreenValuesFromNormalToGPU(SCREEN_WIDTH/2, SCREEN_WIDTH), mapScreenValuesFromNormalToGPU(SCREEN_HEIGHT/2 + HEIGHT, SCREEN_HEIGHT), 0, 1),
				glm::vec4(.5, .5, .5, 1)
				)
			);
	vertices.push_back(
			Vertex(
				glm::vec4(mapScreenValuesFromNormalToGPU(SCREEN_WIDTH/2 + WIDTH, SCREEN_WIDTH), mapScreenValuesFromNormalToGPU(SCREEN_HEIGHT/2 + HEIGHT, SCREEN_HEIGHT), 0, 1),
				glm::vec4(.5, .5, .5, 1)
				)
			);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(1);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GameObject::~GameObject() {
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
    glDeleteVertexArrays(1, &m_vao);
}

void GameObject::Draw() {
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}