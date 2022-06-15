#include <utils/utils.h>
#include <iostream>

GLfloat mapScreenValuesFromNormalToGPU(GLfloat number, GLfloat inMax) {
	return ((number * 2) / inMax) - 1;
}

Vertex makeVertex(GLfloat x, GLfloat y, GLfloat angle) {
	return Vertex(
		glm::vec4(mapScreenValuesFromNormalToGPU(x, SCREEN_WIDTH),
					mapScreenValuesFromNormalToGPU(SCREEN_HEIGHT-y, SCREEN_HEIGHT),
					 0,1),
		glm::vec4(.5, .5, .5, 1),
		glm::vec4(angle, 0, 0, 0)
	);
}

GLfloat generateRandomAngle() {
	GLfloat r3 = LOW_ANGLE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HIGH_ANGLE-LOW_ANGLE)));
    return ((r3 * M_PI) / 180.0f);
}



GLfloat colors[(SCREEN_HEIGHT*SCREEN_WIDTH)*3];

GLfloat* generateRGBScreenHeatMap(float* heatmap) {
	unsigned int i = 0;

    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++) {

			// float density = heatmap[x + SCREEN_WIDTH * (SCREEN_HEIGHT-y)];
			float density = heatmap[x + SCREEN_WIDTH * y];

			// if (density >= 0.99) {
			// 	std::cout << x << ", " << (SCREEN_HEIGHT-y) << std::endl;
			// }

            colors[i] = HEATMAP_R * density;
            colors[i+1] = HEATMAP_G * density;
            colors[i+2] = HEATMAP_B * density;

            i += 3;
        }
    }

	return colors;
}