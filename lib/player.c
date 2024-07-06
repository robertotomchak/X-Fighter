/* Defines the Player of the games */
#include "player.h"

// creates a hit object
Hit *create_hit(short size_x, short size_y, short offset, unsigned short damage)
{
    Hit *h = malloc(sizeof(Hit));
    if (!h)
        return NULL;
    h->size.x = size_x;
    h->size.y = size_y;
    h->offset = offset;
    h->damage = damage;
    return h;
}

// creates a player
Player *create_player(short up, short left, short down, short right, short k_hit_sup, short k_hit_inf, short size_x, short size_y, short speed_x, short jump_speed, Hit *hit_sup, Hit *hit_inf, const char *sprite_path, bool face_right)
{
    Player *p = malloc(sizeof(Player));
    if (!p)
        return NULL;
    p->joystick.up.keycode = up;
    p->joystick.left.keycode = left;
    p->joystick.down.keycode = down;
    p->joystick.right.keycode = right;
    p->joystick.hit_sup.keycode = k_hit_sup;
    p->joystick.hit_inf.keycode = k_hit_inf;
    // starts with no keys pressed
    p->joystick.up.active = 0;
    p->joystick.left.active = 0;
    p->joystick.down.active = 0;
    p->joystick.right.active = 0;
    p->joystick.hit_sup.active = 0;
    p->joystick.hit_inf.active = 0;

    // initialing position to 0
    p->coords.x = 0;
    p->coords.y = 0;

    p->size.x = size_x;
    p->size.y = size_y;

    p->speed.x = speed_x;
    p->speed.y = 0;  // starts on ground
    p->jump_speed = jump_speed;
    p->state = STANDING;

    p->health = MAX_HEALTH; // starts at full health
    p->hit_sup = hit_sup;
    p->hit_inf = hit_inf;
    p->hit_dmg = false;

    p->face_right = face_right;
    // update hits coordinates
    p->hit_sup->coords.x = p->coords.x + p->size.x;
    p->hit_sup->coords.y = p->coords.y - p->size.y * p->hit_sup->offset / 100;
    p->hit_inf->coords.x = p->coords.x + p->size.x;
    p->hit_inf->coords.y = p->coords.y - p->size.y * p->hit_inf->offset / 100;

    p->sprite_status = NORMAL1_SPRITE;
    p->n_frames = 0;
    p->img = create_sprite(sprite_path, SPRITE_WIDTH, SPRITE_HEIGHT, NUM_SPRITES);
    resize_sprite_by_height(p->img, CORRECTION_RATIO * size_y);

    return p;
}

// moves player vertically and updates its speed
void move_player_y(Player *p, short min_y, short max_y, short gravity)
{
    if (p->speed.y > 0)
        p->coords.y = min(max_y, p->coords.y + p->speed.y);
    else
        p->coords.y = max(min_y + p->size.y, p->coords.y + p->speed.y);
    p->speed.y += gravity;
}

// returns true if the players collide horizontally
bool collision_x(Player *p1, Player *p2)
{
    return (p1->coords.x >= p2->coords.x && p1->coords.x < p2->coords.x + p2->size.x) || (p2->coords.x >= p1->coords.x && p2->coords.x < p1->coords.x + p1->size.x);
}

// returns true if the players collide vertically
bool collision_y(Player *p1, Player *p2)
{
    int h1, h2;
    h1 = p1->size.y;
    h2 = p2->size.y;
    if (p1->state == CROUCH)
        h1 /=  2;
    if (p2->state == CROUCH)
        h2 /= 2;

    return (p1->coords.y <= p2->coords.y && p1->coords.y > p2->coords.y - h2) || (p2->coords.y <= p1->coords.y && p2->coords.y > p1->coords.y - h1);
}

// checks if player has been hit
// orientation is true if player that hits is looking to right; false otherwise
bool player_hit(Player *p, Hit *hit, bool orientation) {
    int x1, x2, y1, y2, h;
    bool col_y, col_x;  // collisions

    h = p->size.y;
    if (p->state == CROUCH)
        h /=  2;

    if (orientation) {
        x1 = hit->coords.x;
        x2 = hit->coords.x + hit->size.x;
    }
    else {
        x1 = hit->coords.x - hit->size.x;
        x2 = hit->coords.x;
    }
    y1 = hit->coords.y;
    y2 = hit->coords.y - hit->size.y;

    col_y = (y1 > p->coords.y - h && y1 < p->coords.y) || (y2 > p->coords.y - h && y2 < p->coords.y);
    col_x = (x1 > p->coords.x && x1 < p->coords.x + p->size.x) || (x2 > p->coords.x && x2 < p->coords.x + p->size.x) || (x1 <= p->coords.x && x2 >= p->coords.x + p->size.x);
    return col_x && col_y;
}

// updated player's state
void update_player_state(Player *p, Pair max_screen)
{
    switch (p->state) {
        case STANDING:
            if (p->joystick.down.active)
                p->state = CROUCH;
            else if (p->joystick.up.active) {
                p->state = AIR;
                p->speed.y = -p->jump_speed;
            }
            else if (p->joystick.hit_inf.active) {
                p->state = KICK;
                p->hit_dmg = true;
            }
            else if (p->joystick.hit_sup.active)
                p->state = PREP;
        break;
        case CROUCH:
            if (!p->joystick.down.active)
                p->state = STANDING;
        break;
        case AIR:
            if (p->coords.y >= max_screen.y && p->speed.y > 0) {
                p->state = STANDING;
                p->joystick.up.active = 0;
            }
        break;
        case KICK:
            if (p->n_frames >= KICK_FRAMES) {
                p->state = STANDING;
                p->n_frames = 0;
                p->joystick.hit_inf.active = 0;
                p->hit_dmg = false;
            }
        break;
        case PREP:
            if (p->n_frames >= PREP_FRAMES) {
                p->state = PUNCH;
                p->n_frames = 0;
                p->hit_dmg = true;
            }
        break;
        case PUNCH:
            if (p->n_frames >= PUNCH_FRAMES) {
                p->state = STANDING;
                p->n_frames = 0;
                p->joystick.hit_sup.active = 0;
                p->hit_dmg = false;
            }
        break;
        // should never happen
        default:
            printf("ERROR: NON VALID STATE\n");
        break;
    }
}

// updates player's sprite
void update_player_sprite(Player *p)
{
    // based on player's state
    switch (p->state) {
        case CROUCH:
            p->sprite_status = CROUCH_SPRITE;
        break;
        case AIR:
            p->sprite_status = NORMAL1_SPRITE;
        break;
        case PREP:
            p->sprite_status = PREP_SPRITE;
        break;
        case PUNCH:
            p->sprite_status = PUNCH_SPRITE;
        break;
        case KICK:
            p->sprite_status = KICK_SPRITE;
        break;
        // walking is more complicated
        case STANDING:
            // not walking
            if (!p->joystick.left.active && !p->joystick.right.active) {
                if (p->sprite_status == NORMAL2_SPRITE || p->sprite_status == WALK1_SPRITE)
                    p->sprite_status = NORMAL2_SPRITE;
                else
                    p->sprite_status = NORMAL1_SPRITE;
                p->n_frames = 0;
            }
            // walking and needs to go to next sprite
            else if (p->n_frames >= WALK_FRAMES) {
                p->sprite_status = (p->sprite_status + 1) % TOTAL_WALK_SPRITES;
                p->n_frames = 0;
            }
        break;
        default:
            printf("ERROR: NON VALID STATE!\n");
        break;
    }
    set_sprite_index(p->img, p->sprite_status);
}

// updates player based on event type and key 
// min_screen and max_screen define screen limits
// deals with collision with p_other
void update_player(Player *p, Pair min_screen, Pair max_screen, unsigned int event, unsigned int key, unsigned short gravity, Player *p_other)
{
    int h, h_other;
    h = p->size.y;
    h_other = p_other->size.y;
    if (p->state == CROUCH)
        h /= 2;
    if (p_other->state == CROUCH)
        h_other /= 2;
    // if key pressed or release, update joystick
    if (event == ALLEGRO_EVENT_KEY_DOWN || event == ALLEGRO_EVENT_KEY_UP) {
        if (key == p->joystick.left.keycode)
            p->joystick.left.active ^= 1;
        else if (key == p->joystick.right.keycode)
            p->joystick.right.active ^= 1;
        else if (key == p->joystick.up.keycode && event == ALLEGRO_EVENT_KEY_DOWN)
            p->joystick.up.active = 1;
        else if (key == p->joystick.down.keycode)
            p->joystick.down.active ^= 1;
        else if (key == p->joystick.hit_sup.keycode && event == ALLEGRO_EVENT_KEY_DOWN && p->state == STANDING)
            p->joystick.hit_sup.active = 1;
        else if (key == p->joystick.hit_inf.keycode && event == ALLEGRO_EVENT_KEY_DOWN && p->state == STANDING)
            p->joystick.hit_inf.active = 1;
    }
    update_player_state(p, max_screen);
    update_player_sprite(p);

    // if update screen, move player and update status if necessary
    if (event != ALLEGRO_EVENT_TIMER)
        return;

    // horizontal movement (only allowed in air and standing states)
    if (p->state == STANDING || p->state == AIR) {
        if (p->joystick.left.active) {
            // other player at left
            if (p_other->coords.x + p_other->size.x <= p->coords.x && collision_y(p, p_other))
                min_screen.x = p_other->coords.x + p_other->size.x;
            p->coords.x = max(min_screen.x, p->coords.x - p->speed.x);
        }
        if (p->joystick.right.active) {
            // other player at right
            if (p->coords.x + p->size.x <= p_other->coords.x && collision_y(p, p_other))
                max_screen.x = p_other->coords.x;
            p->coords.x = min(max_screen.x - p->size.x, p->coords.x + p->speed.x);
        }
    }

    // vertical movement (only if player is on the air and not on the top of the other)
    if (p->state == AIR && !(collision_x(p, p_other) && p->coords.y == p_other->coords.y - h_other)) {
        move_player_y(p, min_screen.y, max_screen.y, gravity);
        // collision with other player
        if (collision_x(p, p_other) && collision_y(p, p_other)) {
            // other player is above
            if (p->speed.y < 0)
                p->coords.y = p_other->coords.y + h;
            // other player is below
            else
                p->coords.y = p_other->coords.y - h_other;
            p->speed.y = 0;               
        }
    }

    // update health (if takes damage)
    if (p_other->hit_dmg && p_other->state == PUNCH && player_hit(p, p_other->hit_sup, p_other->face_right)) {
        p->health = max(0, p->health - p_other->hit_sup->damage);
        p_other->hit_dmg = false;
    }
    else if (p_other->hit_dmg && p_other->state == KICK && player_hit(p, p_other->hit_inf, p_other->face_right)) {
        p->health = max(0, p->health - p_other->hit_inf->damage);
        p_other->hit_dmg = false;
    }
        
    // update facing side
    p->face_right = p->coords.x < p_other->coords.x;

    // update hits coordinates
    if (p->face_right) {
        p->hit_sup->coords.x = p->coords.x + p->size.x;
        p->hit_sup->coords.y = p->coords.y - p->size.y * p->hit_sup->offset / 100;
        p->hit_inf->coords.x = p->coords.x + p->size.x;
        p->hit_inf->coords.y = p->coords.y - p->size.y * p->hit_inf->offset / 100;
    }
    else {
        p->hit_sup->coords.x = p->coords.x;
        p->hit_sup->coords.y = p->coords.y - p->size.y * p->hit_sup->offset / 100;
        p->hit_inf->coords.x = p->coords.x;
        p->hit_inf->coords.y = p->coords.y - p->size.y * p->hit_inf->offset / 100;
    }
}

// draws player
void draw_player(Player *p)
{
    p->n_frames += 1;
    int end_hit_x;
    if (p->state == CROUCH)
        al_draw_filled_rectangle(p->coords.x, p->coords.y - p->size.y / 2, p->coords.x + p->size.x, p->coords.y, al_map_rgb(255, 0, 0));
    else
        al_draw_filled_rectangle(p->coords.x, p->coords.y - p->size.y, p->coords.x + p->size.x, p->coords.y, al_map_rgb(255, 0, 0));
    
    if (p->state == PUNCH) {
        end_hit_x = p->face_right? p->hit_sup->coords.x + p->hit_sup->size.x: p->hit_sup->coords.x - p->hit_sup->size.x; 
        al_draw_filled_rectangle(p->hit_sup->coords.x, p->hit_sup->coords.y - p->hit_sup->size.y, end_hit_x, p->hit_sup->coords.y, al_map_rgb(255, 0, 0));
    }
    else if (p->state == KICK) {
        end_hit_x = p->face_right? p->hit_inf->coords.x + p->hit_inf->size.x: p->hit_inf->coords.x - p->hit_inf->size.x;
        al_draw_filled_rectangle(p->hit_inf->coords.x, p->hit_inf->coords.y - p->hit_inf->size.y, end_hit_x, p->hit_inf->coords.y, al_map_rgb(255, 0, 0));
    }
    set_sprite_coords(p->img, p->coords.x + p->size.x / 2, p->coords.y);
    draw_sprite(p->img, !p->face_right);
}

// defines if match is over, and who won
// return -1, 0 or 1:
    // -1: p1 won
    // 0: match is not over
    // 1: p2 won
short round_over(Player *p1, Player *p2)
{
    return (p2->health > 0) - (p1->health > 0);
}

// resets player to given coordinates
// restores health
void reset_player(Player *p, int x, int y)
{
    p->coords.x = x;
    p->coords.y = y;
    p->health = MAX_HEALTH;
    p->state = STANDING;
    p->sprite_status = NORMAL1_SPRITE;
    p->hit_dmg = false;
    p->face_right = true;
    p->speed.y = 0;  // starts on ground
    p->n_frames = 0;
}

// kills player (probably not in a painfull way) by freeing its memory
void kill_player(Player *p)
{
    free(p->hit_inf); p->hit_inf = NULL;
    free(p->hit_sup); p->hit_sup = NULL;
    free(p); p = NULL;
}



