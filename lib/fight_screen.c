/* Defines the screen where the players are fighting */

#include "fight_screen.h"


// creates the screen object
Fight_Screen *create_fight_screen(int width, int height, short n_rounds, Player *p1, Player *p2, int player_x_offset, short scenario, short pause_key)
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

    return screen;
}

// updates screen based on given event
// returns type of exit, based on defines
int update_fight_screen(Fight_Screen *screen, unsigned int event, unsigned int key)
{
    Pair min_screen;
    short score;

    if (event == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return QUIT;

    if (event == ALLEGRO_EVENT_KEY_DOWN && key == screen->pause_key)
        screen->paused = !screen->paused;

    set_pair(&min_screen, 0, 0);
    update_player(screen->p1, min_screen, screen->size, event, key, screen->gravity, screen->p2, screen->paused);
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
    draw_player(screen->p1);
    draw_player(screen->p2);
    draw_var_bar(screen->p1_hp);
    draw_var_bar(screen->p2_hp);
    draw_var_bar(screen->p1_sta);
    draw_var_bar(screen->p2_sta);
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
    al_destroy_font(screen->font); screen->font = NULL;
    free(screen); screen = NULL;
}
