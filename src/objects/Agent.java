package objects;

import java.util.ArrayList;
import java.util.List;

public class Agent extends GameObject {
    
    public static final float SPEED = 2.0f;
    public static final Double SENSOR_ANGLE = -0.610865; // -35 as radian
	public static final float SENSOR_DISTANCE = 15.0f;

    private List<Trail> trailMap = new ArrayList<>();

    public Agent(float x, float y) {
        setX(x);
        setY(y);

        setAngle(generateRandomAngle());
    }

    @Override
    public void setLocation(float x, float y) {
        super.setLocation(x, y);

        trailMap.add(new Trail(x, y));
    }

    public static double generateRandomAngle() {
        return Math.toRadians(0.0f + Math.random() * (360.0f - 0.0f));
    }

    public List<Trail> getTrailMap() {
        return trailMap;
    }

}
