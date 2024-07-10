// Compilação: gcc auxiliary.c player.c main.c -o main $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include "player.h"
#include "fight_screen.h"
#include "select_screen.h"
#include "sprite.h"

#define FPS 30
#define SCREEN_WIDTH 1300
#define SCREEN_HEIGHT 800

#define FONT_VICTORY_SIZE 80
#define FONT_VICTORY_MARGIN_Y 40

// game loop for start screen
// simple enough that doesn't require a special library for it
// returns status (quit or started)
// quit with "x" or ESC
short start_screen(ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *background = al_load_bitmap(START_BACKGROUND_PATH);
    short status = STAY;
    while (status == STAY) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(background, 0, 0, START_IMG_WIDTH, START_IMG_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE))
            status = QUIT;
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            status = START;
    }
    al_destroy_bitmap(background);
    return status;
}

// game loop for select screen
// returns status (quit or selected); puts selected choices in <choices>
short select_loop(ALLEGRO_EVENT_QUEUE *queue, Pair *choices, short *scenario)
{
    ALLEGRO_EVENT event;
    Select_Screen *sscreen = create_select_screen(SCREEN_WIDTH, SCREEN_HEIGHT, ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_LSHIFT,
                        ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_RSHIFT, ALLEGRO_KEY_ENTER, ALLEGRO_KEY_SPACE);
    short status = STAY;
    while (status == STAY) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER)
            draw_select_screen(sscreen);
        status = update_select_screen(sscreen, event.type, event.keyboard.keycode);
    }
    get_choices(sscreen, choices, scenario);
    destroy_select_screen(sscreen);
    return status;
}

// game loop for select screen
// returns status (quit or victory); puts winner (0 or 1) in <p1_won> and its sprite id in <winner>
short fight_loop(ALLEGRO_EVENT_QUEUE *queue, int p1_index, int p2_index, short scenario, bool *p1_won, short *winner)
{
    ALLEGRO_EVENT event;
    short status = STAY;
    // creating players and screens
    char *sprite1, *sprite2;
    switch (p1_index) {
        case RED_PLAYER:
            sprite1 = RED_SPRITE_PATH;
            break;
        case YELLOW_PLAYER:
            sprite1 = YELLOW_SPRITE_PATH;
            break;
        case BLUE_PLAYER:
            sprite1 = BLUE_SPRITE_PATH;
            break;
        case GREEN_PLAYER:
            sprite1 = GREEN_SPRITE_PATH;
            break;
        default:
            printf("ERROR! NON VALID PLAYER\n");
            return 0;
    }
    switch (p2_index) {
        case RED_PLAYER:
            sprite2 = RED_SPRITE_PATH;
            break;
        case YELLOW_PLAYER:
            sprite2 = YELLOW_SPRITE_PATH;
            break;
        case BLUE_PLAYER:
            sprite2 = BLUE_SPRITE_PATH;
            break;
        case GREEN_PLAYER:
            sprite2 = GREEN_SPRITE_PATH;
            break;
        default:
            printf("ERROR! NON VALID PLAYER\n");
            return 0;
    }

    Hit *h_sup1 = create_hit(SIZE_X_SUP, SIZE_Y_SUP, OFFSET_SUP, DMG_SUP);
    Hit *h_inf1 = create_hit(SIZE_X_INF, SIZE_Y_INF, OFFSET_INF, DMG_INF);
    Hit *h_sup2 = create_hit(SIZE_X_SUP, SIZE_Y_SUP, OFFSET_SUP, DMG_SUP);
    Hit *h_inf2 = create_hit(SIZE_X_INF, SIZE_Y_INF, OFFSET_INF, DMG_INF);

    Player *p1 = create_player(ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_S, ALLEGRO_KEY_D, ALLEGRO_KEY_E, ALLEGRO_KEY_R,
                        PLAYER_WIDTH, PLAYER_HEIGHT, SPEED_X, JUMP_SPEED, 
                        h_sup1, h_inf1, sprite1, true, STAMINA_SPEED);
    Player *p2 = create_player(ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_K, ALLEGRO_KEY_L, 
                        PLAYER_WIDTH, PLAYER_HEIGHT, SPEED_X, JUMP_SPEED, 
                        h_sup2, h_inf2, sprite2, false, STAMINA_SPEED); 
    Fight_Screen *fscreen = create_fight_screen(SCREEN_WIDTH, SCREEN_HEIGHT, 3, p1, p2, 50, scenario, ALLEGRO_KEY_ESCAPE);


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
    if (game_over(fscreen) == -1) {
        *p1_won = true;
        *winner = p1_index;
    }
    else {
        *p1_won = false;
        *winner = p2_index;
    }
    destroy_fight_screen(fscreen);
    return status;
}

// game loop for victory screen
// returns status (quit or restart)
short victory_loop(ALLEGRO_EVENT_QUEUE *queue, short winner_id, short winner, short scenario)
{
    ALLEGRO_EVENT event;
    short status = STAY;
    ALLEGRO_BITMAP *background, *player;
    long img_width, img_height;
    int img_flag = winner_id == 1? 0: ALLEGRO_FLIP_HORIZONTAL;
    ALLEGRO_FONT *font = al_load_font(FONT_PATH, FONT_VICTORY_SIZE, 0);
    char *victory_text = winner_id == 1? "Player 1 wins!" : "Player 2 wins!";
    if (scenario == ELETRICAL_SCENARIO) {
        background = al_load_bitmap(SCENARIO_ELETRICAL_PATH);
        img_width = ELETRICAL_IMG_WIDTH;
        img_height = ELETRICAL_IMG_HEIGHT;
    }
    else {
        background = al_load_bitmap(SCENARIO_MOON_PATH);
        img_width = MOON_IMG_WIDTH;
        img_height = MOON_IMG_HEIGHT;
    }

    long player_height = CORRECTION_RATIO * PLAYER_HEIGHT;
    long player_width = player_height * SPRITE_WIDTH / SPRITE_HEIGHT;
    switch (winner) {
        case RED_PLAYER:
            player = al_load_bitmap(RED_SPRITE_PATH);
            break;
        case YELLOW_PLAYER:
            player = al_load_bitmap(YELLOW_SPRITE_PATH);
            break;
        case BLUE_PLAYER:
            player = al_load_bitmap(BLUE_SPRITE_PATH);
            break;
        case GREEN_PLAYER:
            player = al_load_bitmap(GREEN_SPRITE_PATH);
            break;
        default:
            printf("ERROR: NON VALID WINNER PLAYER\n");
            break;
    }

    while (status == STAY) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_draw_scaled_bitmap(background, 0, 0, img_width, img_height, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            al_draw_scaled_bitmap(player, 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, (SCREEN_WIDTH - player_width) / 2, SCREEN_HEIGHT - player_height, player_width, player_height, img_flag);
            al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT * FONT_VICTORY_MARGIN_Y / 100, ALLEGRO_ALIGN_CENTRE, victory_text);
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            status = QUIT;
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                status = QUIT;
            else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                status = BEGIN;
        }
    }
    al_destroy_font(font); font = NULL;
    al_destroy_bitmap(background); background = NULL;
    al_destroy_bitmap(player); player = NULL;

    return status;
}

int main ()
{
    // allegro initializations
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    // auxiliary variables
    short status = BEGIN;
    Pair choices;
    short scenario, winner;
    bool p1_won;

    // main loop
    while (status != QUIT) {
        switch (status) {
            case BEGIN:
                status = start_screen(queue);
                break;
            case START:
                status = select_loop(queue, &choices, &scenario);
                break;
            case SELECTED:
                status = fight_loop(queue, choices.x, choices.y, scenario, &p1_won, &winner);
                break;
            case GAME_OVER:
                status = victory_loop(queue, 2 - p1_won, winner, scenario);
                break;
            default:
                break;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    printf("Ending application\n");
    return 0;
}
