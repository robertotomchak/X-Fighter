// Compilação: gcc auxiliary.c player.c main.c -o main $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "player.h"
#include "fight_screen.h"
#include "select_screen.h"

#define FPS 30
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define GRAVITY 1

#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 100
#define SPEED_X 10
#define JUMP_SPEED 30

#define OFFSET_SUP 70
#define SIZE_X_SUP 40
#define SIZE_Y_SUP 10
#define DMG_SUP 10

#define OFFSET_INF 0
#define SIZE_X_INF 20
#define SIZE_Y_INF 5
#define DMG_INF 5

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

    Hit *h_sup1 = create_hit(SIZE_X_SUP, SIZE_Y_SUP, OFFSET_SUP, DMG_SUP);
    Hit *h_inf1 = create_hit(SIZE_X_INF, SIZE_Y_INF, OFFSET_INF, DMG_INF);
    Hit *h_sup2 = create_hit(SIZE_X_SUP, SIZE_Y_SUP, OFFSET_SUP, DMG_SUP);
    Hit *h_inf2 = create_hit(SIZE_X_INF, SIZE_Y_INF, OFFSET_INF, DMG_INF);

    Player *p1 = create_player(ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_E, ALLEGRO_KEY_R,
                        PLAYER_WIDTH, PLAYER_HEIGHT, SPEED_X, JUMP_SPEED, 
                        h_sup1, h_inf1, al_map_rgb(255, 0, 0));
    Player *p2 = create_player(ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_K, ALLEGRO_KEY_L, 
                        PLAYER_WIDTH, PLAYER_HEIGHT, SPEED_X, JUMP_SPEED, 
                        h_sup2, h_inf2, al_map_rgb(0, 255, 0)); 
    Fight_Screen *fscreen = create_fight_screen(SCREEN_WIDTH, SCREEN_HEIGHT, 3, p1, p2, 50, GRAVITY);
    Select_Screen *sscreen = create_select_screen(SCREEN_WIDTH, SCREEN_HEIGHT, ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_LSHIFT,
                        ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_ENTER);

    short health1 = p1->health, health2 = p2->health, status = STAY;
    printf("%d %d\n", health1, health2);
    while (status == STAY) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER)
            draw_select_screen(sscreen);
        status = update_select_screen(sscreen, event.type, event.keyboard.keycode);
    }
    Pair choices;
    get_choices(sscreen, &choices);
    printf("%d %d\n", choices.x, choices.y);

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroy_fight_screen(fscreen);
    destroy_select_screen(sscreen);
    return 0;
}
