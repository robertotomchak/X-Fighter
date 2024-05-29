/* Defines the screen where the players are fighting */

#include "fight_screen.h"


// creates the screen object
Fight_Screen *create_fight_screen(int width, int height, short n_rounds, Player *p1, Player *p2, int player_x_offset, int gravity)
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
    screen->gravity = gravity;

    reset_player(p1, player_x_offset, height);
    reset_player(p2, width - p2->size.x - player_x_offset, height);

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

    set_pair(&min_screen, 0, 0);
    update_player(screen->p1, min_screen, screen->size, event, key, screen->gravity, screen->p2);
    update_player(screen->p2, min_screen, screen->size, event, key, screen->gravity, screen->p1);

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
    draw_player(screen->p1);
    draw_player(screen->p2);
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
    free(screen); screen = NULL;
}