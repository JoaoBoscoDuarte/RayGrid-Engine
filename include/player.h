#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "map.h"
typedef struct {
    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
} Player;

const Player *player_get_state(void);
void player_init(const EditorState *editor);
void player_movement(const uint8_t *keys, float delta);

#endif
