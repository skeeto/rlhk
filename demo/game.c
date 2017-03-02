typedef int rlhk_algo_map;

#define RLHK_API
#define RLHK_IMPLEMENTATION
#include "../rlhk_tui.h"
#include "../rlhk_rand.h"
#include "../rlhk_algo.h"

#include <stdlib.h>
#include <string.h>

static int width;
static int height;
static int fov_radius = 12;

#define TILE_EMPTY_C   RLHK_TUI_FULL_STOP
#define TILE_EMPTY_A   (RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB)
#define TILE_DIRT_C    RLHK_TUI_MEDIUM_SHADE
#define TILE_DIRT_A    (RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB)
#define TILE_WALL_C    RLHK_TUI_FULL_BLOCK
#define TILE_WALL_A    (RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB)
#define TILE_PLAYER_C  RLHK_TUI_COMMERCIAL_AT
#define TILE_PLAYER_A  (RLHK_TUI_FR | RLHK_TUI_FB | RLHK_TUI_FH)

static char game_map[2][RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static char map_marked[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static char map_visible[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static char map_route[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static long map_distance[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static long map_heuristic[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];

#define IN_MAP(x, y) \
    (x >= 0 && y >= 0 && x < width && y < height)

#define IN_BOUNDS(x, y) \
    (x > 0 && y > 0 && \
     x < width - 1 && y < height - 1)

#define ON_BORDER(x, y) \
    (!x || !y || x == width - 1 || y == height - 1)

static void
map_generate(void)
{
    unsigned long rng[1];
    int x, y, i;

    if (!rlhk_rand_entropy(rng, 4))
        abort();

    memset(game_map, 1, sizeof(game_map));
    for (i = 0; i < width * height / 4; i++) {
        double nx, ny;
        int x, y;
        rlhk_rand_norm(rng, &nx, &ny);
        x = nx * width / 6 + width / 2;
        y = ny * height / 6 + height / 2;
        if (IN_BOUNDS(x, y))
            game_map[1][y][x] = 0;
    }

    for (i = 1; i < 3; i++) {
        for (y = 1; y < height - 1; y++) {
            for (x = 1; x < width - 1; x++) {
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
}

static int draw_dijkstra;

static void
map_draw(int px, int py)
{
    int x, y;
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++) {
            unsigned visible = map_visible[y][x] ? RLHK_TUI_FH : 0;
            if (ON_BORDER(x, y))
                rlhk_tui_putc(x, y, TILE_WALL_C, TILE_WALL_A | visible);
            else if (game_map[0][y][x])
                rlhk_tui_putc(x, y, TILE_DIRT_C, TILE_DIRT_A | visible);
            else {
                unsigned mark = map_marked[y][x] ? RLHK_TUI_BR : 0;
                unsigned c = visible ? TILE_EMPTY_C : ' ';
                mark |= visible;
                if (!draw_dijkstra) {
                    rlhk_tui_putc(x, y, c, TILE_EMPTY_A | mark);
                } else {
                    unsigned a = TILE_EMPTY_A;
                    long dist = map_distance[y][x];
                    if (dist != -1) {
                        c = dist % 10 + '0';
                        a = RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB;
                    }
                    rlhk_tui_putc(x, y, c, a | mark);
                }
            }
        }
    rlhk_tui_putc(px, py, TILE_PLAYER_C, TILE_PLAYER_A);
    if (!rlhk_tui_flush())
        abort();
}

RLHK_ALGO_API
long
rlhk_algo_map_call(rlhk_algo_map map, 
                   enum rlhk_algo_map_method method,
                   int x, int y, long data)
{
    size_t i;
    long *p;
    (void)map;
    if (!IN_MAP(x, y))
        abort();
    switch (method) {
        case RLHK_ALGO_MAP_GET_PASSABLE:
            return game_map[0][y][x] == 0;
        case RLHK_ALGO_MAP_CLEAR_DISTANCE:
            p = &map_distance[0][0];
            for (i = 0; i < sizeof(map_distance) / sizeof(long); i++)
                p[i] = -1;
            return data;
        case RLHK_ALGO_MAP_SET_DISTANCE:
            return (map_distance[y][x] = data);
        case RLHK_ALGO_MAP_GET_DISTANCE:
            return map_distance[y][x];
        case RLHK_ALGO_MAP_SET_HEURISTIC:
            return (map_heuristic[y][x] = data);
        case RLHK_ALGO_MAP_GET_HEURISTIC:
            return map_heuristic[y][x];
        case RLHK_ALGO_MAP_SET_GRADIENT:
            return (map_route[y][x] = data);
        case RLHK_ALGO_MAP_MARK_SHORTEST:
            map_marked[y][x] = 1;
            return map_route[y][x];
        case RLHK_ALGO_MAP_MARK_VISIBLE:
            map_visible[y][x] = 1;
            return game_map[0][y][x] == 0;
    }
    abort();
}

static void
find_path(int x0, int y0, int x1, int y1)
{
    static short buf[1024];
    memset(map_marked, 0, sizeof(map_marked));
    rlhk_algo_shortest(0, x0, y0, x1, y1, buf, sizeof(buf));
}

int
main(void)
{
    int x, y;
    int running = 1;
    
    if (!rlhk_tui_size(&width, &height))
        abort();
    if (width > RLHK_TUI_MAX_WIDTH)
        width = RLHK_TUI_MAX_WIDTH;
    if (height > RLHK_TUI_MAX_HEIGHT)
        height = RLHK_TUI_MAX_HEIGHT;
    x = width / 2;
    y = height / 2;

    if (!rlhk_tui_init(width, height))
        abort();
    map_generate();

    do {
        int k;
        int dx = 0;
        int dy = 0;

        {
            short buf[256];
            long i = rlhk_algo_buf_push(buf, sizeof(buf), 0, x, y);
            rlhk_algo_dijkstra(0, buf, sizeof(buf), i);
            memset(map_visible, 0, sizeof(map_visible));
            rlhk_algo_fov(0, x, y, fov_radius);
        }

        map_draw(x, y);
        if ((k = rlhk_tui_getch()) == -1)
            abort();
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
            case ' ':
                find_path(x, y, width / 2, height / 2);
                break;
            case '+':
                fov_radius++;
                break;
            case '-':
                fov_radius--;
                break;
            case 'x':
                draw_dijkstra  = !draw_dijkstra;
                break;
            case RLHK_TUI_VK_SIGINT:
            case 'q':
                running = 0;
                break;
        }
        if (!game_map[0][y + dy][x + dx]) {
            x += dx;
            y += dy;
        }
    } while (running);

    if (!rlhk_tui_release())
        abort();
    return 0;
}
