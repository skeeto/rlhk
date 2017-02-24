#define RLHK_TUI_IMPLEMENTATION
#include "rlhk_tui.h"

#define TILE_EMPTY_C   ' '
#define TILE_EMPTY_A   0
#define TILE_WALL_C    0x2588u  /* FULL BLOCK */
#define TILE_WALL_A    (RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB | RLHK_TUI_FH)
#define TILE_PLAYER_C  '@'
#define TILE_PLAYER_A  (RLHK_TUI_FR | RLHK_TUI_FB | RLHK_TUI_FH)

static void
clear(void)
{
    int x, y;
    for (y = 0; y < RLHK_TUI_HEIGHT; y++) {
        for (x = 0; x < RLHK_TUI_WIDTH; x++) {
            if (x == 0 ||
                y == 0 ||
                x == RLHK_TUI_WIDTH - 1 ||
                y == RLHK_TUI_HEIGHT - 1) {

                rlhk_tui_putc(x, y, TILE_WALL_C, TILE_WALL_A);
            } else {
                rlhk_tui_putc(x, y, TILE_EMPTY_C, TILE_EMPTY_A);
            }
        }
    }
    rlhk_tui_flush();
}

int
main(void)
{
    int x = RLHK_TUI_WIDTH / 2;
    int y = RLHK_TUI_HEIGHT / 2;
    int running = 1;

    rlhk_tui_init();
    clear();
    rlhk_tui_putc(x, y, TILE_PLAYER_C, TILE_PLAYER_A);

    do {
        int k;
        int dx = 0;
        int dy = 0;

        rlhk_tui_flush();
        k = rlhk_tui_getch();
        switch (k) {
            case RLHK_TUI_VK_L:
            case 'h':
                dx = -1; dy = 0;
                break;
            case RLHK_TUI_VK_R:
            case 'l':
                dx = 1; dy = 0;
                break;
            case RLHK_TUI_VK_U:
            case 'k':
                dx = 0; dy = -1;
                break;
            case RLHK_TUI_VK_D:
            case 'j':
                dx = 0; dy = 1;
                break;
            case RLHK_TUI_VK_UL:
            case 'y':
                dx = -1; dy = -1;
                break;
            case RLHK_TUI_VK_DL:
            case 'b':
                dx = -1; dy = 1;
                break;
            case RLHK_TUI_VK_UR:
            case 'u':
                dx = 1; dy = -1;
                break;
            case RLHK_TUI_VK_DR:
            case 'n':
                dx = 1; dy = 1;
                break;
            case 'q':
                running = 0;
                break;
        }
        if (x + dx > 0 &&
            y + dy > 0 &&
            x + dx < RLHK_TUI_WIDTH - 1 &&
            y + dy < RLHK_TUI_HEIGHT - 1) {

            rlhk_tui_putc(x, y, TILE_EMPTY_C, TILE_EMPTY_A);
            x += dx;
            y += dy;
            rlhk_tui_putc(x, y, TILE_PLAYER_C, TILE_PLAYER_A);
        }
    } while (running);

    rlhk_tui_release();
    return 0;
}
