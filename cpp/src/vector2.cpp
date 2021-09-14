class Vector2I {
public:
    int x;
    int y;
    
    Vector2I(int x, int y) {
        this->x = x;
        this->y = y;
    }

};

class Vector2F {
public:
    float x;
    float y;

    Vector2F(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vector2I asVector2I() {
        Vector2I vi(static_cast<int>(this->x), static_cast<int>(this->y));
        return vi;
    }

};