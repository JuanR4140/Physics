#include <stdio.h>
#include <stdbool.h>
#include "sprite.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

void must_init(bool test, const char *desc){
    if(test) return;
    printf("Could not initialize instance '%s'.\n", desc);
    exit(1);
}

int calculateMouseOffsetX(int startX, int endX){
    int offsetX = startX - endX;
    if(offsetX > 15) offsetX = 15;
    if(offsetX < -15) offsetX = -15;
    return offsetX;
}

int calculateMouseOffsetY(int endY, int startY){
    int offsetY = endY - startY;
    if(offsetY > 15) offsetY = 15;
    if(offsetY < -15) offsetY = -15;
    return offsetY;
}

int main(){
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_mouse(), "mouse");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* display = al_create_display(400, 300);
    must_init(display, "display");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    bool mouseDown = false;
    bool inAir = false;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse;
    int frame = 0;

    // int startX = 50;
    // int startY = 50;
    int startX, startY, endX, endY = 0;

    Sprite ball = { 50, 50, 0, 0, -1, al_map_rgb(0, 0, 255), 16, 16 };

    al_start_timer(timer);
    for(;;){
        frame++;
        if(frame >= 30) frame = 0;

        al_get_mouse_state(&mouse);
        if(mouse.buttons & 1){
            if(!mouseDown && !inAir){
                mouseDown = true;
                startX = mouse.x;
                startY = mouse.y;
                printf("START Mouse: (%d, %d)\n", startX, startY);
            }
        }else{
            if(mouseDown && !inAir){
                mouseDown = false;
                endX = mouse.x;
                endY = mouse.y;
                printf("END Mouse: (%d, %d)\n", endX, endY);
            }
        }

        al_wait_for_event(queue, &event);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:{
                // Frame, do game logic here.

                if(!inAir){
                    if(ball.xvel < 0) ball.xvel += 1;
                    if(ball.xvel > 0) ball.xvel -= 1;
                }


                if(startX && startY && endX && endY){
                    if(!inAir){
                        inAir = true;
                        int x = calculateMouseOffsetX(startX, endX);
                        int y = calculateMouseOffsetY(endY, startY);
                        ball.xvel = x;
                        ball.yvel = -y;
                        startX, startY, endX, endY = 0;
                    }
                }

                inAir = ( ball.yvel ? true : false );

                redraw = true;
                break;
            }

            case ALLEGRO_EVENT_DISPLAY_CLOSE:{
                done = true;
                break;
            }
        }

        if(done) break;

        if(redraw && al_is_event_queue_empty(queue)){
            // Draw to screen based on beforehand calculated logic.
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // al_draw_filled_rectangle(startX, startY, startX + 16, startY + 16, al_map_rgb(255, 0, 0));
            // startX++;
            render(&ball);
            calculatePhysics(&ball);
            keepWithinBorders(&ball);
            al_flip_display();
            redraw = false;
        }

    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    
    return 0;
}