/* screen where players select their players*/

#include "stdio.h"
#include "select_screen.h"

// creates the screen object
Select_Screen *create_select_screen(int width, int height, short up1, short left1, short down1, short right1, short confirm1, short up2, short left2, short down2, short right2, short confirm2, short start_key)
{
    Select_Screen *screen = malloc(sizeof(Select_Screen));
    if (!screen)
        return NULL;
    screen->size.x = width;
    screen->size.y = height;
    screen->n_choices = N_CHOICES;

    screen->select_p1.up = up1;
    screen->select_p1.down = down1;
    screen->select_p1.left = left1;
    screen->select_p1.right = right1;
    screen->select_p1.confirm = confirm1;

    screen->select_p2.up = up2;
    screen->select_p2.down = down2;
    screen->select_p2.left = left2;
    screen->select_p2.right = right2;
    screen->select_p2.confirm = confirm2;

    screen->start_key = start_key;

    // player's heads
    screen->heads[RED_PLAYER] = al_load_bitmap(RED_HEAD_PATH);
    screen->heads[YELLOW_PLAYER] = al_load_bitmap(YELLOW_HEAD_PATH);
    screen->heads[BLUE_PLAYER] = al_load_bitmap(BLUE_HEAD_PATH);
    screen->heads[GREEN_PLAYER] = al_load_bitmap(GREEN_HEAD_PATH);

    // by default, choice 0 is choosen and not confirmed
    screen->selected_choices.x = 0;
    screen->selected_choices.y = 0;
    screen->confirmed.x = 0;
    screen->confirmed.y = 0;

    return screen;
}

// updates player's choice based on what key was pressed
long update_player_choice(Select_Joystick j, long choice, short key, short n_choices)
{
        if (key == j.up && choice - 2 >= 0)
            return choice - 2;
        if (key == j.down && choice + 2 < n_choices)
            return choice + 2;
        if (key == j.left && choice % 2 == 1)
            return choice - 1;
        if (key == j.right && choice % 2 == 0)
            return choice + 1;
    // couldnt do move
    return choice;
}

// updates screen based on given event
// returns type of exit, based on defines
int update_select_screen(Select_Screen *screen, unsigned int event, unsigned int key)
{
    // only updates if its a press event or a quit event
    // if event is quit, return QUIT
    if (event == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return QUIT;
     // if both have confirmed and start key pressed, return SELECTED
    if (screen->confirmed.x && screen->confirmed.y && event == ALLEGRO_EVENT_KEY_DOWN && key == screen->start_key)
        return SELECTED;
    if (event != ALLEGRO_EVENT_KEY_DOWN)
        return STAY;

    // if its click, check cases
    if (key == screen->select_p1.confirm)
        screen->confirmed.x ^= 1;
    else if (key == screen->select_p2.confirm)
        screen->confirmed.y ^= 1;

    if (!screen->confirmed.x)
        screen->selected_choices.x = update_player_choice(screen->select_p1, screen->selected_choices.x, key, screen->n_choices);
    if (!screen->confirmed.y)
        screen->selected_choices.y = update_player_choice(screen->select_p2, screen->selected_choices.y, key, screen->n_choices);
    return STAY;
}

// draws screen
void draw_select_screen(Select_Screen *screen)
{
    long size_big_square = BIG_SQUARE * screen->size.y / 100;
    long size_small_square = SMALL_SQUARE * size_big_square / 100;
    long margin = (size_big_square - 2 * size_small_square) / 3;

    // big square coordinates
    long x1 = (screen->size.x - size_big_square) / 2, y1 = screen->size.y * SQUARE_MARGIN_Y / 100;
    long x2 = x1 + size_big_square, y2 = y1 + size_big_square;

    // colors of squares, based on players choices
    ALLEGRO_COLOR colors[4] = {al_map_rgb(0, 0, 0), al_map_rgb(0, 0, 0), al_map_rgb(0, 0, 0), al_map_rgb(0, 0, 0)};
    colors[screen->selected_choices.x] = al_map_rgb(255, 0, 0);
    colors[screen->selected_choices.y] = al_map_rgb(0, 255, 0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    // big square
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 255));
    // small squares
    al_draw_filled_rectangle(x1+margin, y1+margin, x1+margin+size_small_square, y1+margin+size_small_square, colors[0]);
    al_draw_filled_rectangle(x2-margin-size_small_square, y1+margin, x2-margin, y1+margin+size_small_square, colors[1]);
    al_draw_filled_rectangle(x1+margin, y2-margin-size_small_square, x1+margin+size_small_square, y2-margin, colors[2]);
    al_draw_filled_rectangle(x2-margin-size_small_square, y2-margin-size_small_square, x2-margin, y2-margin, colors[3]);

    // if both players have the same choice, split square in middle
    if (screen->selected_choices.x == screen->selected_choices.y) {
        switch (screen->selected_choices.x) {
            case 0:
                al_draw_filled_rectangle(x1+margin, y1+margin, x1+margin+size_small_square/2, y1+margin+size_small_square, al_map_rgb(255, 0, 0));
                al_draw_filled_rectangle(x1+margin+size_small_square/2, y1+margin, x1+margin+size_small_square, y1+margin+size_small_square, al_map_rgb(0, 255, 0));
                break;
            case 1:
                al_draw_filled_rectangle(x2-margin-size_small_square, y1+margin, x2-margin-size_small_square/2, y1+margin+size_small_square, al_map_rgb(255, 0, 0));
                al_draw_filled_rectangle(x2-margin-size_small_square/2, y1+margin, x2-margin, y1+margin+size_small_square, al_map_rgb(0, 255, 0));
                break;
            case 2:
                al_draw_filled_rectangle(x1+margin, y2-margin-size_small_square, x1+margin+size_small_square/2, y2-margin, al_map_rgb(255, 0, 0));
                al_draw_filled_rectangle(x1+margin+size_small_square/2, y2-margin-size_small_square, x1+margin+size_small_square, y2-margin, al_map_rgb(0, 255, 0));
                break;
            case 3:
            al_draw_filled_rectangle(x2-margin-size_small_square, y2-margin-size_small_square, x2-margin-size_small_square/2, y2-margin, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(x2-margin-size_small_square/2, y2-margin-size_small_square, x2-margin, y2-margin, al_map_rgb(0, 255, 0));
                break;
            default:
                break;
        }
    }

    // draw player's heads
    al_draw_scaled_bitmap(screen->heads[0], 0, 0, PLAYER_HEAD_SIZE, PLAYER_HEAD_SIZE, x1+margin, y1+margin, size_small_square, size_small_square, 0);
    al_draw_scaled_bitmap(screen->heads[1], 0, 0, PLAYER_HEAD_SIZE, PLAYER_HEAD_SIZE, x2-margin-size_small_square, y1+margin, size_small_square, size_small_square, 0);
    al_draw_scaled_bitmap(screen->heads[2], 0, 0, PLAYER_HEAD_SIZE, PLAYER_HEAD_SIZE, x1+margin, y2-margin-size_small_square, size_small_square, size_small_square, 0);
    al_draw_scaled_bitmap(screen->heads[3], 0, 0, PLAYER_HEAD_SIZE, PLAYER_HEAD_SIZE, x2-margin-size_small_square, y2-margin-size_small_square, size_small_square, size_small_square, 0);

    // draw selected players (if any)
    long choice_size = CHOICE_SIZE_FACTOR * size_small_square / 100;
    long choice_x = CHOICE_MARGIN_X * screen->size.x / 100;
    long choice_y = CHOICE_MARGIN_Y * screen->size.y / 100;
    if (screen->confirmed.x) {
        al_draw_scaled_bitmap(screen->heads[screen->selected_choices.x], 0, 0, PLAYER_HEAD_SIZE, PLAYER_HEAD_SIZE, choice_x, choice_y, choice_size, choice_size, 0);
    }
    if (screen->confirmed.y) {
        al_draw_scaled_bitmap(screen->heads[screen->selected_choices.y], 0, 0, PLAYER_HEAD_SIZE, PLAYER_HEAD_SIZE, screen->size.x - choice_x - choice_size, choice_y, choice_size, choice_size, 0);
    }

    al_flip_display();
}

// destroys screen and frees its memory
void destroy_select_screen(Select_Screen *screen)
{
    free(screen); screen = NULL;
}

// gets selected choices and puts on given pair
void get_choices(Select_Screen *screen, Pair *choices)
{
    choices->x = screen->selected_choices.x;
    choices->y = screen->selected_choices.y;
}
