#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

typedef struct Sprite{
    int x;
    int y;
    int xvel;
    int yvel;
    int gravity;
    ALLEGRO_COLOR color;
    int width;
    int height;
} Sprite;

void render(Sprite* _self);
void calculatePhysics(Sprite* _self);
void keepWithinBorders(Sprite* _self);