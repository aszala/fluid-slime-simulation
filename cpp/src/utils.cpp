#include <utils/utils.h>
#include <iostream>

GLfloat mapScreenValuesFromNormalToGPU(GLfloat number, GLfloat inMax) {
	return ((number * 2) / inMax) - 1;
}

Vertex makeVertex(GLfloat x, GLfloat y, GLfloat angle) {
	return Vertex(
		glm::vec4(mapScreenValuesFromNormalToGPU(x, SCREEN_WIDTH),
					mapScreenValuesFromNormalToGPU(y, SCREEN_HEIGHT),
					 0,1),
		glm::vec4(.5, .5, .5, 1),
		glm::vec4(angle, 0, 0, 0)
	);
}

GLfloat generateRandomAngle() {
	GLfloat r3 = LOW_ANGLE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HIGH_ANGLE-LOW_ANGLE)));
    return ((r3 * M_PI) / 180.0f);
}