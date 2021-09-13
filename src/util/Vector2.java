package util;

import java.awt.Point;

public class Vector2 {
    
    public float x = 0, y = 0;
    
    public Vector2() { }

    public Vector2(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public Point asPoint() {
        return new Point((int)x, (int)y);
    }

}
