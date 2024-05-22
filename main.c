// Compilação: gcc auxiliary.c player.c main.c -o main $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)


#include "player.h"

#define FPS 30
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define GRAVITY 1

#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 100
#define SPEED_X 10
#define JUMP_SPEED 30

int main ()
{
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    Player *p1 = create_player(ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_S, ALLEGRO_KEY_D, 50, SCREEN_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, SPEED_X, JUMP_SPEED, 0, al_map_rgb(255, 0, 0));
    Player *p2 = create_player(ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_RIGHT, SCREEN_WIDTH - 50, SCREEN_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, SPEED_X, JUMP_SPEED, 0, al_map_rgb(0, 255, 0)); 
    Pair min_screen, max_screen;
    set_pair(&min_screen, 0, 0);
    set_pair(&max_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (true) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_player(p1);
            draw_player(p2);
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        update_player(p1, min_screen, max_screen, event.type, event.keyboard.keycode, GRAVITY, p2);
        update_player(p2, min_screen, max_screen, event.type, event.keyboard.keycode, GRAVITY, p1);
    }
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    kill_player(p1);
    kill_player(p2);
    return 0;
}
