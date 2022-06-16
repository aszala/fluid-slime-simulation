#include <utils/utils.h>
#include <iostream>
#include <math.h>

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

GLfloat uniform() {
	return (GLfloat)rand() / RAND_MAX;
}

glm::vec2 randPointInCircle(GLfloat center_x, GLfloat center_y, GLfloat radius) {
	GLfloat theta = 2 * M_PI * uniform();
	GLfloat r = sqrt(uniform());

	return glm::vec2(center_x + r * radius * cos(theta), center_y + r * radius * sin(theta));
}

GLfloat angleToCenter(GLfloat y, GLfloat x, GLfloat center_x, GLfloat center_y) {
	return -atan2(center_y - y, center_x - x);
}

void glfwSetWindowCenter(GLFWwindow* window) {
    // Get window position and size
    int window_x, window_y;
    glfwGetWindowPos(window, &window_x, &window_y);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    // Halve the window size and use it to adjust the window position to the center of the window
    window_width *= 0.5;
    window_height *= 0.5;

    window_x += window_width;
    window_y += window_height;

    // Get the list of monitors
    int monitors_length;
    GLFWmonitor **monitors = glfwGetMonitors(&monitors_length);

    if(monitors == NULL) {
        // Got no monitors back
        return;
    }

    // Figure out which monitor the window is in
    GLFWmonitor *owner = NULL;
    int owner_x, owner_y, owner_width, owner_height;

    for(int i = 0; i < monitors_length; i++) {
        // Get the monitor position
        int monitor_x, monitor_y;
        glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

        // Get the monitor size from its video mode
        int monitor_width, monitor_height;
        GLFWvidmode *monitor_vidmode = (GLFWvidmode*) glfwGetVideoMode(monitors[i]);

        if(monitor_vidmode == NULL) {
            // Video mode is required for width and height, so skip this monitor
            continue;

        } else {
            monitor_width = monitor_vidmode->width;
            monitor_height = monitor_vidmode->height;
        }

        // Set the owner to this monitor if the center of the window is within its bounding box
        if((window_x > monitor_x && window_x < (monitor_x + monitor_width)) && (window_y > monitor_y && window_y < (monitor_y + monitor_height))) {
            owner = monitors[i];

            owner_x = monitor_x;
            owner_y = monitor_y;

            owner_width = monitor_width;
            owner_height = monitor_height;
        }
    }

    if(owner != NULL) {
        // Set the window position to the center of the owner monitor
        glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width, owner_y + (owner_height * 0.5) - window_height);
    }
}