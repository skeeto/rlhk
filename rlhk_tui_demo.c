#define RLHK_TUI_IMPLEMENTATION
#include "rlhk_tui.h"

int
main(void)
{
    rlhk_tui_init();
    do {
        int x, y;
        unsigned attr = RLHK_TUI_FR | RLHK_TUI_FG | RLHK_TUI_FB | RLHK_TUI_FH;
        for (y = 0; y < RLHK_TUI_HEIGHT; y++) {
            for (x = 0; x < RLHK_TUI_WIDTH; x++) {
                rlhk_tui_putc(x, y, rand() % 26 + 'a', attr);
            }
        }
        rlhk_tui_flush();
    } while (rlhk_tui_getch() != 'q');
    rlhk_tui_release();
    return 0;
}
