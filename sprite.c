#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "sprite.h"

void render(Sprite* _self){
    Sprite* self = _self;
    al_draw_filled_rectangle(self->x, self->y, self->x+self->width, self->y+self->height, self->color);
}

void calculatePhysics(Sprite* _self){
    Sprite* self = _self;
    self->x += self->xvel;
    self->y += self->yvel;
    self->yvel -= self->gravity;
}

void keepWithinBorders(Sprite* _self){
    Sprite* self = _self;
    if(self->x >= (400 - self->width)) self->xvel = -self->xvel;
    if(self->x <= (0 + self->width)) self->xvel = -self->xvel;
    if(self->y >= (300 - self->height)){self->y = (300 - self->height); self->yvel = 0;}
    if(self->y <= (0 + self->height)) self->y = (0 + self->height);
}