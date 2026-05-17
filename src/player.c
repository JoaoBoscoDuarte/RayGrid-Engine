#include "player.h"
#include "map.h"
#include "log.h"

static Player player;

void player_init(const EditorState *editor) {
    if (!editor) {
        LOG_ERROR("editor is null\n");
        return;
    }

    player.posX = editor->spawnX + 0.5;
    player.posY = editor->spawnY + 0.5;
    player.dirX = -1;
    player.dirY = 0;
    player.planeX = 0;
    player.planeY = 0.66;
    LOG_INFO("Player initialized at {%f, %f}\n", player.posX, player.posY);
}

const Player *player_get_state(void) {
    return &player;
}