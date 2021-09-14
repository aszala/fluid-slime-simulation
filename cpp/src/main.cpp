#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <geom/geom.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void) {
    GLFWwindow *window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Fluid Slime Simulation", NULL, NULL );
    
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 2, 4);
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}