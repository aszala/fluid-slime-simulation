package objects;

import java.awt.Color;

import util.Vector2;

public class GameObject {
    
    private Vector2 location;
    private int color = 255;

    private double angle;

    public GameObject() {
        this.location = new Vector2();
    }

    public void move(float deltaX, float deltaY) {
        location.x += deltaX;
        location.y += deltaY;
    }

    public void setLocation(float x, float y) {
        location.x = x;
        location.y = y;
    }

    public Vector2 getLocation() {
        return location;
    }

    public float predictX(float deltaX) {
        return location.x + deltaX;
    }

    public float predictY(float deltaY) {
        return location.y + deltaY;
    }

    public float getX() {
        return location.x;
    }

    public void setX(float x) {
        location.x = x;
    }

    public float getY() {
        return location.y;
    }

    public void setY(float y) {
        location.y = y;
    }

    public Color getColor() {
        return new Color(color, color, color);
    }

    public int getRawColor() {
        return color;
    }

    public void setColor(int color) {
        this.color = color;
    }

    public double getAngle() {
        return angle;
    }

    public void setAngle(double angle) {
        this.angle = angle;
    }

}
