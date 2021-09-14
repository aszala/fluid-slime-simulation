#ifndef VECTOR2_H
#define VECTOR2_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Vector2I {
public:
    GLint x;
    GLint y;
    
    Vector2I() { }

    Vector2I(GLint x, GLint y) {
        this->x = x;
        this->y = y;
    }

};

class Vector2F {
public:
    GLfloat x;
    GLfloat y;

    Vector2F() { }

    Vector2F(GLfloat newX, GLfloat newY) {
        this->x = newX;
        this->y = newY;
    }
 
    Vector2I asVector2I() {
        Vector2I vi(static_cast<GLint>(this->x), static_cast<GLint>(this->y));
        return vi;
    }

};

#endif