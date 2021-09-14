#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <geom/vector2.h>

class GameObject {

private:
    Vector2F location;
    GLfloat color = 255.0f;
    GLfloat angle;

public:
    GameObject() { }

    GameObject(GLfloat x, GLfloat y) {
        setLocation(x, y);
    }

    void setLocation(GLfloat newX, GLfloat newY) {
        this->location.x = newX;
        this->location.y = newY;
    }

    GLfloat predictX(GLfloat deltaX) {
        return this->location.x + deltaX;
    }

    GLfloat predictY(GLfloat deltaY) {
        return this->location.y + deltaY;
    }

    void moveColor(GLfloat delta) { this->color += delta; }
    void rotate(GLfloat delta) { this->angle += delta; }
    
    void setAngle(GLfloat angle) { this->angle = angle; }

    Vector2F getLocation() { return this->location; }
    GLfloat getX() { return this->location.x; }
    GLfloat getY() { return this->location.y; }
    GLfloat getColor() { return this->color; }
    GLfloat getAngle() { return this->angle; }

};

#endif