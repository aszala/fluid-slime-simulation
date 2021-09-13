package objects;

public class Agent extends GameObject {
    
    public static final float SPEED = 5.0f;

    public Agent(int x, int y) {
        setX(x);
        setY(y);

        double randomAngle = 0.0f + Math.random() * (360.0f - 0.0f);
        
        setAngle(Math.toRadians(randomAngle));
    }

}
