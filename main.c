// Compilação: gcc auxiliary.c player.c main.c -o main $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "player.h"
#include "fight_screen.h"
#include "select_screen.h"

#define FPS 30
#define SCREEN_WIDTH 1120
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

// game loop for start screen
// returns status (quit or started)
short start_screen(ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_EVENT event;
    short status = STAY;
    while (status == STAY) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            status = QUIT;
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            status = START;
    }
    return status;
}

// game loop for select screen
// returns status (quit or selected); puts selected choices in <choices>
short select_loop(ALLEGRO_EVENT_QUEUE *queue, Pair *choices)
{
    ALLEGRO_EVENT event;
    Select_Screen *sscreen = create_select_screen(SCREEN_WIDTH, SCREEN_HEIGHT, ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_LSHIFT,
                        ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_ENTER);
    short status = STAY;
    while (status == STAY) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER)
            draw_select_screen(sscreen);
        status = update_select_screen(sscreen, event.type, event.keyboard.keycode);
    }
    get_choices(sscreen, choices);
    destroy_select_screen(sscreen);
    return status;
}

// game loop for select screen
// returns status (quit or victory); puts winner (0 or 1) in <p1_won>
short fight_loop(ALLEGRO_EVENT_QUEUE *queue, bool *p1_won)
{
    ALLEGRO_EVENT event;
    short status = STAY;
    // creating players and screens
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

    while (status == STAY || status == VICTORY_P1 || status == VICTORY_P2) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER)
            draw_fight_screen(fscreen);
        status = update_fight_screen(fscreen, event.type, event.keyboard.keycode);
        if (status == VICTORY_P1)
            printf("Player 1 won match!\n");
        else if (status == VICTORY_P2)
            printf("Player 2 won match!\n");
    }
    if (game_over(fscreen) == -1)
        *p1_won = true;
    else
        *p1_won = false;
    destroy_fight_screen(fscreen);
    return status;
}

int main ()
{
    // allegro initializations
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    // auxiliary variables
    short status;
    Pair choices;
    bool p1_won;

    // game loop
    status = start_screen(queue);
    if (status == QUIT) {
        printf("Quitting game\n");
        return 0;
    }
    status = select_loop(queue, &choices);
    if (status == QUIT) {
        printf("Quitting game\n");
        return 0;
    }
    printf("Players Selected: %d %d\n", choices.x, choices.y);
    status = fight_loop(queue, &p1_won);
    if (status == QUIT) {
        printf("Quitting game\n");
        return 0;
    }
    else if (p1_won) {
        printf("Player 1 won the game!\n");
    }
    else {
        printf("Player 2 won the game!\n");
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    printf("Ending application\n");
    return 0;
}
