#define RLHK_API static
#define RLHK_IMPLEMENTATION
#include "../rlhk_tui.h"
#include "../rlhk_rand.h"

#include <string.h>
#include <stdlib.h>

#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((b) > (a) ? (b) : (a))
#define WHITE  RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB | RLHK_TUI_FH
#define GREEN  RLHK_TUI_FG | RLHK_TUI_FH

enum func {
    FUNC_NORM
};

static void
bin_fill(unsigned long *bins,
         int nbins,
         long n,
         unsigned long *rng,
         enum func func)
{
    int i, j;

    for (i = 0; i < n; i++) {
        switch (func) {
            case FUNC_NORM: {
                double s[2];
                rlhk_rand_norm(rng, s + 0, s + 1);
                for (j = 0; j < 2; j++) {
                    int v = s[j] * nbins / 6.0 + nbins / 2.0 + 0.5;
                    if (v >= 0 && v < nbins)
                        bins[v]++;
                }
            } break;
        }
    }
}

static void
bin_draw(unsigned long *bins, int width, int height)
{
    int x, y;
    unsigned long bin_max = 0;

    for (x = 0; x < width; x++)
        if (bins[x] > bin_max)
            bin_max = bins[x];

    for (x = 0; x < width; x++) {
        int s = height * bins[x] / (double)bin_max;
        for (y = 0; y < height; y++) {
            unsigned c = ' ';
            if (y <= s - 4)
                c = RLHK_TUI_FULL_BLOCK;
            else if (y <= s - 3)
                c = RLHK_TUI_DARK_SHADE;
            else if (y <= s - 2)
                c = RLHK_TUI_MEDIUM_SHADE;
            else if (y <= s - 1)
                c = RLHK_TUI_LIGHT_SHADE;
            rlhk_tui_putc(x, height - y - 1, c, WHITE);
        }
    }
    rlhk_tui_flush();
}

static void
print(int width, const char *s)
{
    int len = strlen(s);
    int x;
    for (x = 0; s[x]; x++)
        rlhk_tui_putc(width - len + x, 0, s[x], GREEN);
}

int
main(void)
{
    int width = 0;
    int height = 0;
    unsigned long rng[1];
    unsigned long bins[RLHK_TUI_MAX_WIDTH];
    int i;

    if (!rlhk_tui_size(&width, &height))
        abort();
    width = MIN(width, RLHK_TUI_MAX_WIDTH);
    height = MIN(height, RLHK_TUI_MAX_HEIGHT);
    rlhk_tui_init(width, height);
    rlhk_rand_entropy(rng, 4);

    memset(bins, 0, sizeof(bins));
    for (i = 0; i < 10000; i++) {
        bin_fill(bins, width, 100, rng, FUNC_NORM);
        bin_draw(bins, width, height);
    }
    print(width, "Press any key ...");
    rlhk_tui_flush();
    rlhk_tui_getch();

    rlhk_tui_release();

    return 0;
}
