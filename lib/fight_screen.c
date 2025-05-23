/* Defines the screen where the players are fighting */

#include "fight_screen.h"


// creates the screen object
Fight_Screen *create_fight_screen(int width, int height, short n_rounds, Player *p1, Player *p2, int player_x_offset, short scenario, short pause_key, bool bot_player)
{
    Fight_Screen *screen = malloc(sizeof(Fight_Screen));
    if (!screen)
        return NULL;

    screen->size.x = width;
    screen->size.y = height;
    screen->n_rounds = n_rounds;
    screen->score.x = 0;
    screen->score.y = 0;
    screen->p1 = p1;
    screen->p2 = p2;
    screen->player_x_offset = player_x_offset;

    reset_player(p1, player_x_offset, height);
    reset_player(p2, width - p2->size.x - player_x_offset, height);

    screen->p1_hp = create_health_bar(HP_BAR_MARGIN_X * width / 100, HP_BAR_MARGIN_Y * height / 100, HP_BAR_SIZE_X * width / 100, HP_BAR_SIZE_Y * height / 100, false, MAX_HEALTH);
    screen->p2_hp = create_health_bar(width - HP_BAR_MARGIN_X * width / 100, HP_BAR_MARGIN_Y * height / 100, HP_BAR_SIZE_X * width / 100, HP_BAR_SIZE_Y * height / 100, true, MAX_HEALTH);

    screen->p1_sta = create_stamina_bar(STA_BAR_MARGIN_X * width / 100, STA_BAR_MARGIN_Y * height / 100, STA_BAR_SIZE_X * width / 100, STA_BAR_SIZE_Y * height / 100, false, MAX_STAMINA);
    screen->p2_sta = create_stamina_bar(width - STA_BAR_MARGIN_X * width / 100, STA_BAR_MARGIN_Y * height / 100, STA_BAR_SIZE_X * width / 100, STA_BAR_SIZE_Y * height / 100, true, MAX_STAMINA);
    
    if (scenario == ELETRICAL_SCENARIO) {
        screen->background = al_load_bitmap(SCENARIO_ELETRICAL_PATH);
        screen->gravity = GRAVITY_ELETRICAL;
        screen->source_img_size.x = ELETRICAL_IMG_WIDTH;
        screen->source_img_size.y = ELETRICAL_IMG_HEIGHT;
    }
    else {
        screen->background = al_load_bitmap(SCENARIO_MOON_PATH);
        screen->gravity = GRAVITY_MOON;
        screen->source_img_size.x = MOON_IMG_WIDTH;
        screen->source_img_size.y = MOON_IMG_HEIGHT;
    }

    screen->font = al_load_font(FONT_PATH, FONT_SIZE_PAUSED, 0);
    screen->paused = false;
    screen->pause_key = pause_key;

    screen->points = al_load_bitmap(BABY_PATH);

    screen->bot_player = bot_player;

    return screen;
}

// updates screen based on given event
// returns type of exit, based on defines
int update_fight_screen(Fight_Screen *screen, unsigned int event, unsigned int key)
{
    Pair min_screen;
    short score;
    int bot_event, bot_key;

    if (event == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return QUIT;

    if (event == ALLEGRO_EVENT_KEY_DOWN && key == screen->pause_key)
        screen->paused = !screen->paused;

    set_pair(&min_screen, 0, 0);
    update_player(screen->p1, min_screen, screen->size, event, key, screen->gravity, screen->p2, screen->paused);
    
    if (screen->bot_player) {
        if (event == ALLEGRO_EVENT_TIMER)
            update_player(screen->p2, min_screen, screen->size, event, key, screen->gravity, screen->p1, screen->paused);
        bot_play(screen->p2, screen->p1, &bot_event, &bot_key);
        if (bot_event != -1)
            update_player(screen->p2, min_screen, screen->size, bot_event, bot_key, screen->gravity, screen->p1, screen->paused);
    }
    else
        update_player(screen->p2, min_screen, screen->size, event, key, screen->gravity, screen->p1, screen->paused);
    if (!screen->paused) {
        update_var_bar(screen->p1_hp, screen->p1->health);
        update_var_bar(screen->p2_hp, screen->p2->health);
        update_var_bar(screen->p1_sta, screen->p1->stamina);
        update_var_bar(screen->p2_sta, screen->p2->stamina);
    }

    score = round_over(screen->p1, screen->p2);
    if (score != 0) {
        if (score == -1)
            screen->score.x += 1;
        else
            screen->score.y += 1;
        reset_player(screen->p1, screen->player_x_offset, screen->size.y);
        reset_player(screen->p2, screen->size.x - screen->player_x_offset - screen->p2->size.x, screen->size.y);
        printf("%d %d\n", screen->score.x, screen->score.y);
        
        // check if game is over
        if (game_over(screen))
            return GAME_OVER;
        else if (score == -1)
            return VICTORY_P1;
        else
            return VICTORY_P2;
    }
    return STAY;
}

// draws screen
void draw_fight_screen(Fight_Screen *screen)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(screen->background, 0, 0, screen->source_img_size.x, screen->source_img_size.y, 0, 0, screen->size.x, screen->size.y, 0);
    draw_player(screen->p1, SHOW_HITBOXES);
    draw_player(screen->p2, SHOW_HITBOXES);
    draw_var_bar(screen->p1_hp);
    draw_var_bar(screen->p2_hp);
    draw_var_bar(screen->p1_sta);
    draw_var_bar(screen->p2_sta);
    // draws points of player 1
    int i = 0;
    long points_x = screen->size.x * HP_BAR_MARGIN_X / 100, points_y = screen->size.y * POINTS_MARGIN_Y / 100, points_size = screen->size.y * POINTS_SIZE / 100;
    for (i = 0; i < screen->score.x; i++) {
        al_draw_scaled_bitmap(screen->points, 0, 0, BABY_IMG_WIDTH, BABY_IMG_HEIGHT, points_x, points_y, points_size, points_size, 0);
        points_x += points_size;
    }
    // draws points of player 2
    points_x  = screen->size.x - screen->size.x * HP_BAR_MARGIN_X / 100 - points_size;
    for (i = 0; i < screen->score.y; i++) {
        al_draw_scaled_bitmap(screen->points, 0, 0, BABY_IMG_WIDTH, BABY_IMG_HEIGHT, points_x, points_y, points_size, points_size, 0);
        points_x -= points_size;
    }

    if (screen->paused)
        al_draw_text(screen->font, al_map_rgb(255, 255, 255), screen->size.x / 2, FONT_PAUSED_MARGIN_Y * screen->size.y / 100, ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
    al_flip_display();
}

// checks if game is over
// returns -1, 0 or 1:
//  -1: p1 won most rounds
//   0: rounds not over (or tie, only possible if n_rounds is even)
//   1: p2 won most rounds
int game_over(Fight_Screen *screen)
{
    if (screen->score.x > screen->n_rounds / 2)
        return -1;
    if (screen->score.y > screen->n_rounds / 2)
        return 1;
    return 0;
}

// destroys screen and frees its memory
void destroy_fight_screen(Fight_Screen *screen)
{
    kill_player(screen->p1);
    kill_player(screen->p2);
	destroy_var_bar(screen->p1_hp);
	destroy_var_bar(screen->p2_hp);
    al_destroy_bitmap(screen->background); screen->background = NULL;
    al_destroy_bitmap(screen->points); screen->points = NULL;
    al_destroy_font(screen->font); screen->font = NULL;
    free(screen); screen = NULL;
}
