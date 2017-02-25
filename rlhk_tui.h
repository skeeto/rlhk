/* Roguelike Header Kit : Text User Interface
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Provides portable (POSIX and Win32) functions for efficiently
 * rendering 16 colors of text to a terminal / console. No third-party
 * libraries, such as ncurses, are required. The maximum display size
 * is determined by the compile-time values RLHK_TUI_MAX_WIDTH and
 * RLHK_TUI_MAX_HEIGHT. You may define to your own before including
 * this module.
 *
 * For POSIX systems, the terminal is assumed to support ANSI escapes
 * and UTF-8 encoding.
 *
 * This library does not require nor use stdio.h. This may be useful
 * in some circumstances, such as builds with a static libc.
 *
 * To get the implementation, define RLHK_TUI_IMPLEMENTATION before
 * including this file. You may define your own RLHK_TUI_API to
 * control the linkage and/or visibility of the API.
 *
 * Functions:
 *   - rlhk_tui_init
 *   - rlhk_tui_release
 *   - rlhk_tui_putc
 *   - rlhk_tui_flush
 *   - rlhk_tui_getch
 *   - rlhk_tui_title
 *   - rlhk_tui_size
 */
#ifndef RLHK_TUI_H
#define RLHK_TUI_H

#ifndef RLHK_TUI_API
#  define RLHK_TUI_API
#endif

#ifndef RLHK_TUI_MAX_WIDTH
#  define RLHK_TUI_MAX_WIDTH  80
#endif

#ifndef RLHK_TUI_MAX_HEIGHT
#  define RLHK_TUI_MAX_HEIGHT 24
#endif

/**
 * Must be called before most TUI functions.
 *
 * Both rlhk_tui_title() and rlhk_tui_size() may be called without an
 * initialized display.
 *
 * To properly restore the terminal/console state, you must call
 * rlhk_tui_release() before exiting the process. This function may be
 * called again after rlhk_tui_release().
 *
 * The maximum width and height are determined by RLHK_TUI_MAX_WIDTH
 * and RLHK_TUI_MAX_HEIGHT. On Windows, the width cannot be greater
 * than 80.
 *
 * Returns 1 on success, 0 on failure.
 */
RLHK_TUI_API
int rlhk_tui_init(int width, int height);

/**
 * Reverse of rlhk_tui_init(), restoring the terminal / console.
 */
RLHK_TUI_API
void rlhk_tui_release(void);

#define RLHK_TUI_FR (1u << 0)  /* foreground red       */
#define RLHK_TUI_FG (1u << 1)  /* foreground green     */
#define RLHK_TUI_FB (1u << 2)  /* foreground blue      */
#define RLHK_TUI_FH (1u << 3)  /* foreground highlight */
#define RLHK_TUI_BR (1u << 4)  /* background red       */
#define RLHK_TUI_BG (1u << 5)  /* background green     */
#define RLHK_TUI_BB (1u << 6)  /* background blue      */
#define RLHK_TUI_BH (1u << 7)  /* background highlight */

/**
 * Put a 16-bit character at x, y with specific fore/background color.
 *
 * The limiting factor is the Win32 console, which can only display a
 * small subset of the BMP. Choose your characters wisely.
 *
 * An attribute is an 8-bit value composed of ORing the defined values
 * above. There are 16 available colors each for foreground and
 * background. The specific channel values (i.e. shades) are
 * determined by the terminal / console configuration.
 *
 * Characters are not immediately visible on the display. Instead they
 * are written to a background buffer, which is written to the display
 * with rlhk_tui_flush().
 */
RLHK_TUI_API
void rlhk_tui_putc(int x, int y, unsigned c, unsigned attr);

/**
 * Flush all changes to the display.
 *
 * Calls to rlhk_tui_putc() are not visible until this call. You
 * probably want to call this function before rlhk_tui_getch().
 */
RLHK_TUI_API
void rlhk_tui_flush(void);

#define RLHK_TUI_VK_U       321     /* up         */
#define RLHK_TUI_VK_D       322     /* down       */
#define RLHK_TUI_VK_L       324     /* left       */
#define RLHK_TUI_VK_R       323     /* right      */
#define RLHK_TUI_VK_UL      305     /* up-left    */
#define RLHK_TUI_VK_DL      308     /* down-left  */
#define RLHK_TUI_VK_UR      309     /* up-right   */
#define RLHK_TUI_VK_DR      310     /* down-right */
#define RLHK_TUI_VK_SIGINT  3       /* ctrl-c     */

/**
 * Blocks and returns a key press from the user.
 *
 * Limited to ASCII characters, but some special inputs, such as
 * directional arrow keys, have the above definitions.
 */
RLHK_TUI_API
int rlhk_tui_getch(void);

/**
 * Sets the terminal / console window title.
 */
RLHK_TUI_API
void rlhk_tui_title(const char *);

/**
 * Gets the current terminal / console size.
 *
 * Useful for checking the size of the display in order to dynamically
 * scale the display, abort with an error for being too small, or to
 * wait until the screen is resized.
 */
RLHK_TUI_API
void rlhk_tui_size(int *width, int *height);

/* Implementation */
#ifdef RLHK_TUI_IMPLEMENTATION

static int rlhk_tui_width;
static int rlhk_tui_height;

#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

struct termios rlhk_tui_termios_orig;

/* The last written display. */
static unsigned short rlhk_tui_oldc[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static unsigned char rlhk_tui_olda[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];

/* Characters to be written on the next flush. */
static unsigned short rlhk_tui_bufc[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static unsigned char rlhk_tui_bufa[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];

static unsigned char *
rlhk_tui_itoa(unsigned char *p, int v)
{
    int len = !v;
    int i;
    for (i = v; i; i /= 10)
        len++;
    for (i = len - 1; i >= 0; i--, v /= 10)
        p[i] = v % 10 + '0';
    return p + len;
}

RLHK_TUI_API
int
rlhk_tui_init(int width, int height)
{
    struct termios raw;
    char init[] = {
        "\x1b[2J"   /* Clear the screen. */
        "\x1b[?25l" /* Hide the cursor. */
    };
    rlhk_tui_width = width;
    rlhk_tui_height = height;
    tcgetattr(STDIN_FILENO, &rlhk_tui_termios_orig);
    memcpy(&raw, &rlhk_tui_termios_orig, sizeof(raw));
    raw.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    raw.c_oflag &= ~OPOST;
    raw.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    raw.c_cflag &= ~(CSIZE|PARENB);
    raw.c_cflag |= CS8;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    return write(STDIN_FILENO, init, sizeof(init) - 1) == sizeof(init) - 1;
}

RLHK_TUI_API
void
rlhk_tui_release(void)
{
    char finish[24] =
        "\x1b[?25h"  /* Restore cursor visibility. */
        "\x1b[";     /* Move cursor just outside drawing region. */
    unsigned char *f = (unsigned char *)finish + strlen(finish);
    char *p = (char *)rlhk_tui_itoa(f, rlhk_tui_height);
    strcpy(p, ";0H" "\x1b[0m\n");  /* Disable color/style. */
    tcsetattr(STDIN_FILENO, TCSANOW, &rlhk_tui_termios_orig);
    write(STDIN_FILENO, finish, sizeof(finish) - 1);
    memset(rlhk_tui_oldc, 0, sizeof(rlhk_tui_oldc));
}

RLHK_TUI_API
void
rlhk_tui_putc(int x, int y, unsigned c, unsigned attr)
{
    rlhk_tui_bufc[y][x] = c;
    rlhk_tui_bufa[y][x] = attr;
}

static int
rlhk_tui_ucs2_to_8(unsigned ucs2, unsigned char *utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        return 1;
    }
    if (ucs2 < 0x800) {
        utf8[0] = 0xc0 | ((ucs2 & 0x07c0) >> 6);
        utf8[1] = 0x80 |  (ucs2 & 0x003f);
        return 2;
    }
    utf8[0] = 0xe0 | ((ucs2 & 0xf000) >> 12);
    utf8[1] = 0x80 | ((ucs2 & 0x0fc0) >>  6);
    utf8[2] = 0x80 |  (ucs2 & 0x003f);
    return 3;
}

RLHK_TUI_API
void
rlhk_tui_flush(void)
{
    int x, y;
    static unsigned char buf[RLHK_TUI_MAX_HEIGHT * RLHK_TUI_MAX_WIDTH *
                             (9 + 3 + 4 + 10) / 2];
    unsigned char *p = buf;
    unsigned last_a = -1u;
    int cx = -1;
    int cy = -1;
    for (y = 0; y < rlhk_tui_height; y++) {
        for (x = 0; x < rlhk_tui_width; x++) {
            unsigned short *oc = &rlhk_tui_oldc[y][x];
            unsigned char *oa = &rlhk_tui_olda[y][x];
            unsigned c = rlhk_tui_bufc[y][x];
            unsigned a = rlhk_tui_bufa[y][x];
            if (*oc != c || *oa != a) {
                /* Move to location. */
                if (x != cx || y != cy) {
                    *p++ = 0x1b;
                    *p++ = '[';
                    p = rlhk_tui_itoa(p, y + 1);
                    *p++ = ';';
                    p = rlhk_tui_itoa(p, x + 1);
                    *p++ = 'H';
                    last_a = -1u;
                }
                /* Apply colors. */
                if (a != last_a) {
                    int fg = ((a >> 0) & 0x07) + (a & 0x08 ? 90  : 30);
                    int bg = ((a >> 4) & 0x07) + (a & 0x80 ? 100 : 40);
                    *p++ = 0x1b;
                    *p++ = '[';
                    p = rlhk_tui_itoa(p, fg);
                    *p++ = ';';
                    p = rlhk_tui_itoa(p, bg);
                    *p++ = 'm';
                    last_a = a;
                }
                p += rlhk_tui_ucs2_to_8(c, p);
                cx = x + 1;
                cy = y;
                *oc = c;
                *oa = a;
            }
        }
    }
    write(STDOUT_FILENO, buf, p - buf);
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

static CHAR_INFO rlhk_tui_buf[RLHK_TUI_MAX_HEIGHT * RLHK_TUI_MAX_WIDTH];
static HANDLE rlhk_tui_out;
static HANDLE rlhk_tui_in;
static DWORD rlhk_tui_mode_orig;

RLHK_TUI_API
int
rlhk_tui_init(int width, int height)
{
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SMALL_RECT window = {0, 0, 0, 0};
    COORD size;
    DWORD mode;
    rlhk_tui_width = width;
    rlhk_tui_height = height;
    rlhk_tui_out = GetStdHandle(STD_OUTPUT_HANDLE);
    rlhk_tui_in  = GetStdHandle(STD_INPUT_HANDLE);

    /* Adjust console input and cursor. */
    SetConsoleCursorInfo(rlhk_tui_out, &info);
    GetConsoleMode(rlhk_tui_in, &rlhk_tui_mode_orig);
    mode = rlhk_tui_mode_orig & ~ENABLE_PROCESSED_INPUT;
    SetConsoleMode(rlhk_tui_in, mode);

    /* Adjust window size. */
    window.Bottom = height - 1;
    window.Right = width - 1;
    if (!SetConsoleWindowInfo(rlhk_tui_out, TRUE, &window))
        return 0;

    /* Adjust buffer size. */
    size.X = width;
    size.Y = height;
    if (!SetConsoleScreenBufferSize(rlhk_tui_out, size))
        return 0;

    return 1;
}

RLHK_TUI_API
void
rlhk_tui_release(void)
{
    CONSOLE_CURSOR_INFO info = {100, TRUE};
    COORD coord = {0, 0};
    coord.Y = rlhk_tui_height;
    SetConsoleCursorInfo(rlhk_tui_out, &info);
    SetConsoleCursorPosition(rlhk_tui_out, coord);
    SetConsoleMode(rlhk_tui_in, rlhk_tui_mode_orig);
}

RLHK_TUI_API
void
rlhk_tui_putc(int x, int y, unsigned c, unsigned attr)
{
    rlhk_tui_buf[y * rlhk_tui_width + x].Char.UnicodeChar = c;
    rlhk_tui_buf[y * rlhk_tui_width + x].Attributes =
        (attr & 0xaa) | ((attr >> 2) & 0x11) | ((attr << 2) & 0x44);
}

RLHK_TUI_API
void
rlhk_tui_flush(void)
{
    COORD origin = {0, 0};
    SMALL_RECT area = {0, 0, 0, 0};
    COORD size;
    area.Right = rlhk_tui_width;
    area.Bottom = rlhk_tui_height;
    size.X = rlhk_tui_width;
    size.Y = rlhk_tui_height;
    WriteConsoleOutputW(rlhk_tui_out, rlhk_tui_buf, size, origin, &area);
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
