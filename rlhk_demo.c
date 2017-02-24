#define RLHK_TUI_IMPLEMENTATION
#define RLHK_RAND_IMPLEMENTATION
#include "rlhk_tui.h"
#include "rlhk_rand.h"

#define TILE_EMPTY_C   ' '
#define TILE_EMPTY_A   0
#define TILE_DIRT_C    0x2592u  /* MEDIUM SHADE */
#define TILE_DIRT_A    (RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB)
#define TILE_WALL_C    0x2588u  /* FULL BLOCK */
#define TILE_WALL_A    (RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB | RLHK_TUI_FH)
#define TILE_PLAYER_C  '@'
#define TILE_PLAYER_A  (RLHK_TUI_FR | RLHK_TUI_FB | RLHK_TUI_FH)

static char game_map[2][RLHK_TUI_HEIGHT][RLHK_TUI_WIDTH];

#define IN_BOUNDS(x, y) \
    (x > 0 && y > 0 && x < RLHK_TUI_WIDTH - 1 && y < RLHK_TUI_HEIGHT - 1)

#define ON_BORDER(x, y) \
    (!x || !y || x == RLHK_TUI_WIDTH - 1 || y == RLHK_TUI_HEIGHT - 1)

static void
map_generate(void)
{
    unsigned long rng[1];
    int x, y, i;

    rlhk_rand_entropy(rng, 4);

    memset(game_map, 1, sizeof(game_map));
    for (i = 0; i < RLHK_TUI_WIDTH * RLHK_TUI_HEIGHT / 4; i++) {
        double nx, ny;
        int x, y;
        rlhk_rand_norm(rng, &nx, &ny);
        x = nx * RLHK_TUI_WIDTH / 6 + RLHK_TUI_WIDTH / 2;
        y = ny * RLHK_TUI_HEIGHT / 6 + RLHK_TUI_HEIGHT / 2;
        if (IN_BOUNDS(x, y))
            game_map[1][y][x] = 0;
    }

    for (i = 1; i < 3; i++) {
        for (y = 1; y < RLHK_TUI_HEIGHT - 1; y++) {
            for (x = 1; x < RLHK_TUI_WIDTH - 1; x++) {
                int sum = 
                    game_map[i % 2][y - 1][x + 1] +
                    game_map[i % 2][y - 1][x + 0] +
                    game_map[i % 2][y - 1][x - 1] +
                    game_map[i % 2][y + 0][x + 1] +
                    game_map[i % 2][y + 0][x - 1] +
                    game_map[i % 2][y + 1][x + 1] +
                    game_map[i % 2][y + 1][x + 0] +
                    game_map[i % 2][y + 1][x - 1];
                game_map[(i + 1) % 2][y][x] = sum > 6;
            }
        }
    }

    for (y = 0; y < RLHK_TUI_HEIGHT; y++)
        for (x = 0; x < RLHK_TUI_WIDTH; x++)
            if (ON_BORDER(x, y))
                rlhk_tui_putc(x, y, TILE_WALL_C, TILE_WALL_A);
            else if (game_map[0][y][x])
                rlhk_tui_putc(x, y, TILE_DIRT_C, TILE_DIRT_A);
            else 
                rlhk_tui_putc(x, y, TILE_EMPTY_C, TILE_EMPTY_A);
    rlhk_tui_flush();
}

int
main(void)
{
    int x = RLHK_TUI_WIDTH / 2;
    int y = RLHK_TUI_HEIGHT / 2;
    int running = 1;

    rlhk_tui_init();
    map_generate();
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
            case RLHK_TUI_VK_SIGINT:
            case 'q':
                running = 0;
                break;
        }
        if (!game_map[0][y + dy][x + dx]) {
            rlhk_tui_putc(x, y, TILE_EMPTY_C, TILE_EMPTY_A);
            x += dx;
            y += dy;
            rlhk_tui_putc(x, y, TILE_PLAYER_C, TILE_PLAYER_A);
        }
    } while (running);

    rlhk_tui_release();
    return 0;
}
