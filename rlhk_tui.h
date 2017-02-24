#ifndef RLHK_TUI_H
#define RLHK_TUI_H

#ifndef RLHK_TUI_API
#  define RLHK_TUI_API
#endif

#ifndef RLHK_TUI_WIDTH
#  define RLHK_TUI_WIDTH  80
#  define RLHK_TUI_HEIGHT 24
#endif

#define RLHK_TUI_FR (1u << 0)  /* foreground red       */
#define RLHK_TUI_FG (1u << 1)  /* foreground green     */
#define RLHK_TUI_FB (1u << 2)  /* foreground blue      */
#define RLHK_TUI_FH (1u << 3)  /* foreground highlight */
#define RLHK_TUI_BR (1u << 4)  /* background red       */
#define RLHK_TUI_BG (1u << 5)  /* background green     */
#define RLHK_TUI_BB (1u << 6)  /* background blue      */
#define RLHK_TUI_BH (1u << 7)  /* background highlight */

#define RLHK_TUI_VK_U  321     /* up         */
#define RLHK_TUI_VK_D  322     /* down       */
#define RLHK_TUI_VK_L  324     /* left       */
#define RLHK_TUI_VK_R  323     /* right      */
#define RLHK_TUI_VK_UL 305     /* up-left    */
#define RLHK_TUI_VK_DL 308     /* down-left  */
#define RLHK_TUI_VK_UR 309     /* up-right   */
#define RLHK_TUI_VK_DR 310     /* down-right */

RLHK_TUI_API
void rlhk_tui_init(void);

RLHK_TUI_API
void rlhk_tui_release(void);

RLHK_TUI_API
void rlhk_tui_putc(int x, int y, unsigned c, unsigned attr);

RLHK_TUI_API
void rlhk_tui_flush(void);

RLHK_TUI_API
int rlhk_tui_getch(void);

RLHK_TUI_API
void rlhk_tui_title(const char *);

RLHK_TUI_API
void rlhk_tui_size(int *width, int *height);

#ifdef RLHK_TUI_IMPLEMENTATION

#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

static unsigned short rlhk_tui_bufc[RLHK_TUI_HEIGHT][RLHK_TUI_WIDTH];
static unsigned char rlhk_tui_bufa[RLHK_TUI_HEIGHT][RLHK_TUI_WIDTH];
static unsigned long rlhk_tui_dirty[RLHK_TUI_HEIGHT * RLHK_TUI_WIDTH];
struct termios rlhk_tui_termios_orig;

RLHK_TUI_API
void
rlhk_tui_init(void)
{
    struct termios raw;
    write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
    tcgetattr(STDIN_FILENO, &rlhk_tui_termios_orig);
    memcpy(&raw, &rlhk_tui_termios_orig, sizeof(raw));
    raw.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    raw.c_oflag &= ~OPOST;
    raw.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    raw.c_cflag &= ~(CSIZE|PARENB);
    raw.c_cflag |= CS8;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    write(STDIN_FILENO, "\x1b[?25l", 6);
}

RLHK_TUI_API
void
rlhk_tui_release(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &rlhk_tui_termios_orig);
    write(STDIN_FILENO, "\x1b[?25h\x1b[m\n", 10);
}

RLHK_TUI_API
void
rlhk_tui_putc(int x, int y, unsigned c, unsigned attr)
{
    int i = y * RLHK_TUI_WIDTH + x;
    rlhk_tui_bufc[y][x] = c;
    rlhk_tui_bufa[y][x] = attr;
    rlhk_tui_dirty[i / (sizeof(unsigned long) * 8)] |=
        1UL << (i % (sizeof(unsigned long) * 8));
}

static int
rlhk_tui_ucs2_to_8(unsigned ucs2, unsigned char *utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = 0;
        return 1;
    }
    if (ucs2 < 0x800) {
        utf8[0] = 0xc0 | ((ucs2 & 0x07c0) >> 6);
        utf8[1] = 0x80 |  (ucs2 & 0x003f);
        utf8[2] = 0;
        return 2;
    }
    utf8[0] = 0xe0 | ((ucs2 & 0xf000) >> 12);
    utf8[1] = 0x80 | ((ucs2 & 0x0fc0) >>  6);
    utf8[2] = 0x80 |  (ucs2 & 0x003f);
    utf8[3] = 0;
    return 3;
}

RLHK_TUI_API
void
rlhk_tui_flush(void)
{
    int x, y;
    static unsigned char buf[RLHK_TUI_HEIGHT * RLHK_TUI_WIDTH *
                             (9 + 3 + 4 + 10)];
    unsigned char *p = buf + 3;
    buf[0] = 0x1b;
    buf[1] = '[';
    buf[2] = 'H';
    for (y = 0; y < RLHK_TUI_HEIGHT; y++) {
        for (x = 0; x < RLHK_TUI_WIDTH; x++) {
            int i = y * RLHK_TUI_WIDTH + x;
            if (rlhk_tui_dirty[i / (sizeof(unsigned long) * 8)] &
                1UL << (i % (sizeof(unsigned long) * 8))) {

                /* TODO: color */
                /* TODO: efficient */
                unsigned c = rlhk_tui_bufc[y][x];
                p += rlhk_tui_ucs2_to_8(c, p);
            }
        }
    }
    write(STDOUT_FILENO, buf, p - buf);
    memset(rlhk_tui_dirty, 0, sizeof(rlhk_tui_dirty));
}

RLHK_TUI_API
int
rlhk_tui_getch(void)
{
    int r;
    unsigned char c;
    if ((r = read(STDIN_FILENO, &c, sizeof(c))) < 0) {
        return r;
    } else if (c == '\x1b') {
        unsigned char code[2];
        (void)read(STDIN_FILENO, code, sizeof(code));
        return code[1] + 256;
    } else {
        if (c == 3) {
            /* TODO: SIGINT */
            rlhk_tui_release();
            exit(EXIT_FAILURE);
        }
        return c;
    }
}

RLHK_TUI_API
void
rlhk_tui_title(const char *title)
{
    char a = '\a';
    write(STDIN_FILENO, "\x1b]2;", 4);
    write(STDIN_FILENO, title, strlen(title));
    write(STDIN_FILENO, &a, 1);
}

RLHK_TUI_API
void
rlhk_tui_size(int *width, int *height)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
}

#elif defined(_WIN32)
#include <windows.h>
#include <conio.h>

static CHAR_INFO rlhk_tui_buf[RLHK_TUI_HEIGHT][RLHK_TUI_WIDTH];
static HANDLE rlhk_tui_out;
static HANDLE rlhk_tui_in;

RLHK_TUI_API
void
rlhk_tui_init(void)
{
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    rlhk_tui_out = GetStdHandle(STD_OUTPUT_HANDLE);
    rlhk_tui_in  = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleCursorInfo(rlhk_tui_out, &info);
}

RLHK_TUI_API
void
rlhk_tui_release(void)
{
    CONSOLE_CURSOR_INFO info = {100, TRUE};
    SetConsoleCursorInfo(rlhk_tui_out, &info);
}

RLHK_TUI_API
void
rlhk_tui_putc(int x, int y, unsigned c, unsigned attr)
{
    rlhk_tui_buf[y][x].Char.UnicodeChar = c;
    rlhk_tui_buf[y][x].Attributes =
        (attr & 0xaa) | ((attr >> 2) & 0x11) | ((attr << 2) & 0x44);
}

RLHK_TUI_API
void
rlhk_tui_flush(void)
{
    COORD size = {RLHK_TUI_WIDTH, RLHK_TUI_HEIGHT};
    COORD origin = {0, 0};
    SMALL_RECT area = {0, 0, RLHK_TUI_WIDTH, RLHK_TUI_HEIGHT};
    WriteConsoleOutputW(rlhk_tui_out, rlhk_tui_buf[0], size, origin, &area);
}

RLHK_TUI_API
int
rlhk_tui_getch(void)
{
    int result = getch();
    if (result != 0xE0 && result != 0x00) {
        return result;
    } else {
        result = getch();
        switch (result) {
        case 72:
            return RLHK_TUI_VK_U;
        case 80:
            return RLHK_TUI_VK_D;
        case 75:
            return RLHK_TUI_VK_L;
        case 77:
            return RLHK_TUI_VK_R;
        case 71:
            return RLHK_TUI_VK_UL;
        case 73:
            return RLHK_TUI_VK_UR;
        case 79:
            return RLHK_TUI_VK_DL;
        case 81:
            return RLHK_TUI_VK_DR;
        default:
            return result + 256;
        }
    }
}

RLHK_TUI_API
void
rlhk_tui_title(const char *title)
{
    SetConsoleTitle(title);
}

RLHK_TUI_API
void
rlhk_tui_size(int *width, int *height)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

#endif /* _WIN32 */
#endif /* RLHK_TUI_IMPLEMENTATION */
#endif /* RLHK_TUI_H */
