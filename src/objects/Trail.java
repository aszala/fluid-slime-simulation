package objects;

public class Trail extends GameObject {
    
    public static final float DEATH_RATE = 10.0f;

    private float lifeSpan = 255;

    public Trail(float x, float y) {
        setLocation(x, y);

        setColor((int)lifeSpan);
    }

    public void update() {
        lifeSpan -= DEATH_RATE;

        setColor((int)lifeSpan);
    }

    public float getLifeSpan() {
        return lifeSpan;
    }

}
