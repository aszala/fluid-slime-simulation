package objects;

public class GameObject {
    
    private float x, y;
    private int color = 255;

    private double angle;

    public GameObject() { }

    public void move(float deltaX, float deltaY) {
        this.x += deltaX;
        this.y += deltaY;
    }

    public float getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public float getY() {
        return y;
    }

    public void setY(float y) {
        this.y = y;
    }

    public int getColor() {
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
