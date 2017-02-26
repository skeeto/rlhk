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
#  define RLHK_TUI_MAX_HEIGHT 25
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
 * Returns 1 on success, 0 on failure. Call rlhk_tui_release()
 * regardless of this result.
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

#if (defined(__unix__) || defined(__APPLE__)) && !defined(__DJGPP__)
#define RLHK_TUI_SPACE                                              0x000020U
#define RLHK_TUI_EXCLAMATION_MARK                                   0x000021U
#define RLHK_TUI_QUOTATION_MARK                                     0x000022U
#define RLHK_TUI_NUMBER_SIGN                                        0x000023U
#define RLHK_TUI_DOLLAR_SIGN                                        0x000024U
#define RLHK_TUI_PERCENT_SIGN                                       0x000025U
#define RLHK_TUI_AMPERSAND                                          0x000026U
#define RLHK_TUI_APOSTROPHE                                         0x000027U
#define RLHK_TUI_LEFT_PARENTHESIS                                   0x000028U
#define RLHK_TUI_RIGHT_PARENTHESIS                                  0x000029U
#define RLHK_TUI_ASTERISK                                           0x00002aU
#define RLHK_TUI_PLUS_SIGN                                          0x00002bU
#define RLHK_TUI_COMMA                                              0x00002cU
#define RLHK_TUI_HYPHEN_MINUS                                       0x00002dU
#define RLHK_TUI_FULL_STOP                                          0x00002eU
#define RLHK_TUI_SOLIDUS                                            0x00002fU
#define RLHK_TUI_DIGIT_ZERO                                         0x000030U
#define RLHK_TUI_DIGIT_ONE                                          0x000031U
#define RLHK_TUI_DIGIT_TWO                                          0x000032U
#define RLHK_TUI_DIGIT_THREE                                        0x000033U
#define RLHK_TUI_DIGIT_FOUR                                         0x000034U
#define RLHK_TUI_DIGIT_FIVE                                         0x000035U
#define RLHK_TUI_DIGIT_SIX                                          0x000036U
#define RLHK_TUI_DIGIT_SEVEN                                        0x000037U
#define RLHK_TUI_DIGIT_EIGHT                                        0x000038U
#define RLHK_TUI_DIGIT_NINE                                         0x000039U
#define RLHK_TUI_COLON                                              0x00003aU
#define RLHK_TUI_SEMICOLON                                          0x00003bU
#define RLHK_TUI_LESS_THAN_SIGN                                     0x00003cU
#define RLHK_TUI_EQUALS_SIGN                                        0x00003dU
#define RLHK_TUI_GREATER_THAN_SIGN                                  0x00003eU
#define RLHK_TUI_QUESTION_MARK                                      0x00003fU
#define RLHK_TUI_COMMERCIAL_AT                                      0x000040U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A                             0x000041U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_B                             0x000042U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_C                             0x000043U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_D                             0x000044U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_E                             0x000045U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_F                             0x000046U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_G                             0x000047U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_H                             0x000048U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_I                             0x000049U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_J                             0x00004aU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_K                             0x00004bU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_L                             0x00004cU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_M                             0x00004dU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_N                             0x00004eU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_O                             0x00004fU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_P                             0x000050U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Q                             0x000051U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_R                             0x000052U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_S                             0x000053U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_T                             0x000054U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_U                             0x000055U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_V                             0x000056U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_W                             0x000057U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_X                             0x000058U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Y                             0x000059U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Z                             0x00005aU
#define RLHK_TUI_LEFT_SQUARE_BRACKET                                0x00005bU
#define RLHK_TUI_REVERSE_SOLIDUS                                    0x00005cU
#define RLHK_TUI_RIGHT_SQUARE_BRACKET                               0x00005dU
#define RLHK_TUI_CIRCUMFLEX_ACCENT                                  0x00005eU
#define RLHK_TUI_LOW_LINE                                           0x00005fU
#define RLHK_TUI_GRAVE_ACCENT                                       0x000060U
#define RLHK_TUI_LATIN_SMALL_LETTER_A                               0x000061U
#define RLHK_TUI_LATIN_SMALL_LETTER_B                               0x000062U
#define RLHK_TUI_LATIN_SMALL_LETTER_C                               0x000063U
#define RLHK_TUI_LATIN_SMALL_LETTER_D                               0x000064U
#define RLHK_TUI_LATIN_SMALL_LETTER_E                               0x000065U
#define RLHK_TUI_LATIN_SMALL_LETTER_F                               0x000066U
#define RLHK_TUI_LATIN_SMALL_LETTER_G                               0x000067U
#define RLHK_TUI_LATIN_SMALL_LETTER_H                               0x000068U
#define RLHK_TUI_LATIN_SMALL_LETTER_I                               0x000069U
#define RLHK_TUI_LATIN_SMALL_LETTER_J                               0x00006aU
#define RLHK_TUI_LATIN_SMALL_LETTER_K                               0x00006bU
#define RLHK_TUI_LATIN_SMALL_LETTER_L                               0x00006cU
#define RLHK_TUI_LATIN_SMALL_LETTER_M                               0x00006dU
#define RLHK_TUI_LATIN_SMALL_LETTER_N                               0x00006eU
#define RLHK_TUI_LATIN_SMALL_LETTER_O                               0x00006fU
#define RLHK_TUI_LATIN_SMALL_LETTER_P                               0x000070U
#define RLHK_TUI_LATIN_SMALL_LETTER_Q                               0x000071U
#define RLHK_TUI_LATIN_SMALL_LETTER_R                               0x000072U
#define RLHK_TUI_LATIN_SMALL_LETTER_S                               0x000073U
#define RLHK_TUI_LATIN_SMALL_LETTER_T                               0x000074U
#define RLHK_TUI_LATIN_SMALL_LETTER_U                               0x000075U
#define RLHK_TUI_LATIN_SMALL_LETTER_V                               0x000076U
#define RLHK_TUI_LATIN_SMALL_LETTER_W                               0x000077U
#define RLHK_TUI_LATIN_SMALL_LETTER_X                               0x000078U
#define RLHK_TUI_LATIN_SMALL_LETTER_Y                               0x000079U
#define RLHK_TUI_LATIN_SMALL_LETTER_Z                               0x00007aU
#define RLHK_TUI_LEFT_CURLY_BRACKET                                 0x00007bU
#define RLHK_TUI_VERTICAL_LINE                                      0x00007cU
#define RLHK_TUI_RIGHT_CURLY_BRACKET                                0x00007dU
#define RLHK_TUI_TILDE                                              0x00007eU
#define RLHK_TUI_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA                0x0087c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_DIAERESIS                0x00bcc3U
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_ACUTE                    0x00a9c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX               0x00a2c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_DIAERESIS                0x00a4c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_GRAVE                    0x00a0c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE               0x00a5c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_C_WITH_CEDILLA                  0x00a7c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX               0x00aac3U
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_DIAERESIS                0x00abc3U
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_GRAVE                    0x00a8c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_DIAERESIS                0x00afc3U
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX               0x00aec3U
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_GRAVE                    0x00acc3U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS              0x0084c3U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE             0x0085c3U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_E_WITH_ACUTE                  0x0089c3U
#define RLHK_TUI_LATIN_SMALL_LIGATURE_AE                            0x00a6c3U
#define RLHK_TUI_LATIN_CAPITAL_LIGATURE_AE                          0x0086c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX               0x00b4c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_DIAERESIS                0x00b6c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_GRAVE                    0x00b2c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX               0x00bbc3U
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_GRAVE                    0x00b9c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS                0x00bfc3U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS              0x0096c3U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS              0x009cc3U
#define RLHK_TUI_CENT_SIGN                                          0x00a2c2U
#define RLHK_TUI_POUND_SIGN                                         0x00a3c2U
#define RLHK_TUI_YEN_SIGN                                           0x00a5c2U
#define RLHK_TUI_PESETA_SIGN                                        0xa782e2U
#define RLHK_TUI_LATIN_SMALL_LETTER_F_WITH_HOOK                     0x0092c6U
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_ACUTE                    0x00a1c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_ACUTE                    0x00adc3U
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_ACUTE                    0x00b3c3U
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_ACUTE                    0x00bac3U
#define RLHK_TUI_LATIN_SMALL_LETTER_N_WITH_TILDE                    0x00b1c3U
#define RLHK_TUI_LATIN_CAPITAL_LETTER_N_WITH_TILDE                  0x0091c3U
#define RLHK_TUI_FEMININE_ORDINAL_INDICATOR                         0x00aac2U
#define RLHK_TUI_MASCULINE_ORDINAL_INDICATOR                        0x00bac2U
#define RLHK_TUI_INVERTED_QUESTION_MARK                             0x00bfc2U
#define RLHK_TUI_REVERSED_NOT_SIGN                                  0x908ce2U
#define RLHK_TUI_NOT_SIGN                                           0x00acc2U
#define RLHK_TUI_VULGAR_FRACTION_ONE_HALF                           0x00bdc2U
#define RLHK_TUI_VULGAR_FRACTION_ONE_QUARTER                        0x00bcc2U
#define RLHK_TUI_INVERTED_EXCLAMATION_MARK                          0x00a1c2U
#define RLHK_TUI_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK          0x00abc2U
#define RLHK_TUI_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK         0x00bbc2U
#define RLHK_TUI_LIGHT_SHADE                                        0x9196e2U
#define RLHK_TUI_MEDIUM_SHADE                                       0x9296e2U
#define RLHK_TUI_DARK_SHADE                                         0x9396e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL                        0x8294e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT               0xa494e2U
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE       0xa195e2U
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE       0xa295e2U
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE           0x9695e2U
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE           0x9595e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT              0xa395e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL                       0x9195e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT                  0x9795e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT                    0x9d95e2U
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE             0x9c95e2U
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE             0x9b95e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT                   0x9094e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT                    0x9494e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL               0xb494e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL             0xac94e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT              0x9c94e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_HORIZONTAL                      0x8094e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL         0xbc94e2U
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE      0x9e95e2U
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE      0x9f95e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT                   0x9a95e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT                 0x9495e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL              0xa995e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL            0xa695e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT             0xa095e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_HORIZONTAL                     0x9095e2U
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL        0xac95e2U
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE       0xa795e2U
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE       0xa895e2U
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE     0xa495e2U
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE     0xa595e2U
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE            0x9995e2U
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE            0x9895e2U
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE          0x9295e2U
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE          0x9395e2U
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE 0xab95e2U
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE 0xaa95e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_LEFT                     0x9894e2U
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT                  0x8c94e2U
#define RLHK_TUI_FULL_BLOCK                                         0x8896e2U
#define RLHK_TUI_LOWER_HALF_BLOCK                                   0x8496e2U
#define RLHK_TUI_LEFT_HALF_BLOCK                                    0x8c96e2U
#define RLHK_TUI_RIGHT_HALF_BLOCK                                   0x9096e2U
#define RLHK_TUI_UPPER_HALF_BLOCK                                   0x8096e2U
#define RLHK_TUI_GREEK_SMALL_LETTER_ALPHA                           0x00b1ceU
#define RLHK_TUI_LATIN_SMALL_LETTER_SHARP_S                         0x009fc3U
#define RLHK_TUI_GREEK_CAPITAL_LETTER_GAMMA                         0x0093ceU
#define RLHK_TUI_GREEK_SMALL_LETTER_PI                              0x0080cfU
#define RLHK_TUI_GREEK_CAPITAL_LETTER_SIGMA                         0x00a3ceU
#define RLHK_TUI_GREEK_SMALL_LETTER_SIGMA                           0x0083cfU
#define RLHK_TUI_MICRO_SIGN                                         0x00b5c2U
#define RLHK_TUI_GREEK_SMALL_LETTER_TAU                             0x0084cfU
#define RLHK_TUI_GREEK_CAPITAL_LETTER_PHI                           0x00a6ceU
#define RLHK_TUI_GREEK_CAPITAL_LETTER_THETA                         0x0098ceU
#define RLHK_TUI_GREEK_CAPITAL_LETTER_OMEGA                         0x00a9ceU
#define RLHK_TUI_GREEK_SMALL_LETTER_DELTA                           0x00b4ceU
#define RLHK_TUI_INFINITY                                           0x9e88e2U
#define RLHK_TUI_GREEK_SMALL_LETTER_PHI                             0x0086cfU
#define RLHK_TUI_GREEK_SMALL_LETTER_EPSILON                         0x00b5ceU
#define RLHK_TUI_INTERSECTION                                       0xa988e2U
#define RLHK_TUI_IDENTICAL_TO                                       0xa189e2U
#define RLHK_TUI_PLUS_MINUS_SIGN                                    0x00b1c2U
#define RLHK_TUI_GREATER_THAN_OR_EQUAL_TO                           0xa589e2U
#define RLHK_TUI_LESS_THAN_OR_EQUAL_TO                              0xa489e2U
#define RLHK_TUI_TOP_HALF_INTEGRAL                                  0xa08ce2U
#define RLHK_TUI_BOTTOM_HALF_INTEGRAL                               0xa18ce2U
#define RLHK_TUI_DIVISION_SIGN                                      0x00b7c3U
#define RLHK_TUI_ALMOST_EQUAL_TO                                    0x8889e2U
#define RLHK_TUI_DEGREE_SIGN                                        0x00b0c2U
#define RLHK_TUI_BULLET_OPERATOR                                    0x9988e2U
#define RLHK_TUI_MIDDLE_DOT                                         0x00b7c2U
#define RLHK_TUI_SQUARE_ROOT                                        0x9a88e2U
#define RLHK_TUI_SUPERSCRIPT_LATIN_SMALL_LETTER_N                   0xbf81e2U
#define RLHK_TUI_SUPERSCRIPT_TWO                                    0x00b2c2U
#define RLHK_TUI_BLACK_SQUARE                                       0xa096e2U
#elif defined(_WIN32)
#define RLHK_TUI_SPACE                                              0x0020
#define RLHK_TUI_EXCLAMATION_MARK                                   0x0021
#define RLHK_TUI_QUOTATION_MARK                                     0x0022
#define RLHK_TUI_NUMBER_SIGN                                        0x0023
#define RLHK_TUI_DOLLAR_SIGN                                        0x0024
#define RLHK_TUI_PERCENT_SIGN                                       0x0025
#define RLHK_TUI_AMPERSAND                                          0x0026
#define RLHK_TUI_APOSTROPHE                                         0x0027
#define RLHK_TUI_LEFT_PARENTHESIS                                   0x0028
#define RLHK_TUI_RIGHT_PARENTHESIS                                  0x0029
#define RLHK_TUI_ASTERISK                                           0x002a
#define RLHK_TUI_PLUS_SIGN                                          0x002b
#define RLHK_TUI_COMMA                                              0x002c
#define RLHK_TUI_HYPHEN_MINUS                                       0x002d
#define RLHK_TUI_FULL_STOP                                          0x002e
#define RLHK_TUI_SOLIDUS                                            0x002f
#define RLHK_TUI_DIGIT_ZERO                                         0x0030
#define RLHK_TUI_DIGIT_ONE                                          0x0031
#define RLHK_TUI_DIGIT_TWO                                          0x0032
#define RLHK_TUI_DIGIT_THREE                                        0x0033
#define RLHK_TUI_DIGIT_FOUR                                         0x0034
#define RLHK_TUI_DIGIT_FIVE                                         0x0035
#define RLHK_TUI_DIGIT_SIX                                          0x0036
#define RLHK_TUI_DIGIT_SEVEN                                        0x0037
#define RLHK_TUI_DIGIT_EIGHT                                        0x0038
#define RLHK_TUI_DIGIT_NINE                                         0x0039
#define RLHK_TUI_COLON                                              0x003a
#define RLHK_TUI_SEMICOLON                                          0x003b
#define RLHK_TUI_LESS_THAN_SIGN                                     0x003c
#define RLHK_TUI_EQUALS_SIGN                                        0x003d
#define RLHK_TUI_GREATER_THAN_SIGN                                  0x003e
#define RLHK_TUI_QUESTION_MARK                                      0x003f
#define RLHK_TUI_COMMERCIAL_AT                                      0x0040
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A                             0x0041
#define RLHK_TUI_LATIN_CAPITAL_LETTER_B                             0x0042
#define RLHK_TUI_LATIN_CAPITAL_LETTER_C                             0x0043
#define RLHK_TUI_LATIN_CAPITAL_LETTER_D                             0x0044
#define RLHK_TUI_LATIN_CAPITAL_LETTER_E                             0x0045
#define RLHK_TUI_LATIN_CAPITAL_LETTER_F                             0x0046
#define RLHK_TUI_LATIN_CAPITAL_LETTER_G                             0x0047
#define RLHK_TUI_LATIN_CAPITAL_LETTER_H                             0x0048
#define RLHK_TUI_LATIN_CAPITAL_LETTER_I                             0x0049
#define RLHK_TUI_LATIN_CAPITAL_LETTER_J                             0x004a
#define RLHK_TUI_LATIN_CAPITAL_LETTER_K                             0x004b
#define RLHK_TUI_LATIN_CAPITAL_LETTER_L                             0x004c
#define RLHK_TUI_LATIN_CAPITAL_LETTER_M                             0x004d
#define RLHK_TUI_LATIN_CAPITAL_LETTER_N                             0x004e
#define RLHK_TUI_LATIN_CAPITAL_LETTER_O                             0x004f
#define RLHK_TUI_LATIN_CAPITAL_LETTER_P                             0x0050
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Q                             0x0051
#define RLHK_TUI_LATIN_CAPITAL_LETTER_R                             0x0052
#define RLHK_TUI_LATIN_CAPITAL_LETTER_S                             0x0053
#define RLHK_TUI_LATIN_CAPITAL_LETTER_T                             0x0054
#define RLHK_TUI_LATIN_CAPITAL_LETTER_U                             0x0055
#define RLHK_TUI_LATIN_CAPITAL_LETTER_V                             0x0056
#define RLHK_TUI_LATIN_CAPITAL_LETTER_W                             0x0057
#define RLHK_TUI_LATIN_CAPITAL_LETTER_X                             0x0058
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Y                             0x0059
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Z                             0x005a
#define RLHK_TUI_LEFT_SQUARE_BRACKET                                0x005b
#define RLHK_TUI_REVERSE_SOLIDUS                                    0x005c
#define RLHK_TUI_RIGHT_SQUARE_BRACKET                               0x005d
#define RLHK_TUI_CIRCUMFLEX_ACCENT                                  0x005e
#define RLHK_TUI_LOW_LINE                                           0x005f
#define RLHK_TUI_GRAVE_ACCENT                                       0x0060
#define RLHK_TUI_LATIN_SMALL_LETTER_A                               0x0061
#define RLHK_TUI_LATIN_SMALL_LETTER_B                               0x0062
#define RLHK_TUI_LATIN_SMALL_LETTER_C                               0x0063
#define RLHK_TUI_LATIN_SMALL_LETTER_D                               0x0064
#define RLHK_TUI_LATIN_SMALL_LETTER_E                               0x0065
#define RLHK_TUI_LATIN_SMALL_LETTER_F                               0x0066
#define RLHK_TUI_LATIN_SMALL_LETTER_G                               0x0067
#define RLHK_TUI_LATIN_SMALL_LETTER_H                               0x0068
#define RLHK_TUI_LATIN_SMALL_LETTER_I                               0x0069
#define RLHK_TUI_LATIN_SMALL_LETTER_J                               0x006a
#define RLHK_TUI_LATIN_SMALL_LETTER_K                               0x006b
#define RLHK_TUI_LATIN_SMALL_LETTER_L                               0x006c
#define RLHK_TUI_LATIN_SMALL_LETTER_M                               0x006d
#define RLHK_TUI_LATIN_SMALL_LETTER_N                               0x006e
#define RLHK_TUI_LATIN_SMALL_LETTER_O                               0x006f
#define RLHK_TUI_LATIN_SMALL_LETTER_P                               0x0070
#define RLHK_TUI_LATIN_SMALL_LETTER_Q                               0x0071
#define RLHK_TUI_LATIN_SMALL_LETTER_R                               0x0072
#define RLHK_TUI_LATIN_SMALL_LETTER_S                               0x0073
#define RLHK_TUI_LATIN_SMALL_LETTER_T                               0x0074
#define RLHK_TUI_LATIN_SMALL_LETTER_U                               0x0075
#define RLHK_TUI_LATIN_SMALL_LETTER_V                               0x0076
#define RLHK_TUI_LATIN_SMALL_LETTER_W                               0x0077
#define RLHK_TUI_LATIN_SMALL_LETTER_X                               0x0078
#define RLHK_TUI_LATIN_SMALL_LETTER_Y                               0x0079
#define RLHK_TUI_LATIN_SMALL_LETTER_Z                               0x007a
#define RLHK_TUI_LEFT_CURLY_BRACKET                                 0x007b
#define RLHK_TUI_VERTICAL_LINE                                      0x007c
#define RLHK_TUI_RIGHT_CURLY_BRACKET                                0x007d
#define RLHK_TUI_TILDE                                              0x007e
#define RLHK_TUI_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA                0x00c7
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_DIAERESIS                0x00fc
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_ACUTE                    0x00e9
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX               0x00e2
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_DIAERESIS                0x00e4
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_GRAVE                    0x00e0
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE               0x00e5
#define RLHK_TUI_LATIN_SMALL_LETTER_C_WITH_CEDILLA                  0x00e7
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX               0x00ea
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_DIAERESIS                0x00eb
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_GRAVE                    0x00e8
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_DIAERESIS                0x00ef
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX               0x00ee
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_GRAVE                    0x00ec
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS              0x00c4
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE             0x00c5
#define RLHK_TUI_LATIN_CAPITAL_LETTER_E_WITH_ACUTE                  0x00c9
#define RLHK_TUI_LATIN_SMALL_LIGATURE_AE                            0x00e6
#define RLHK_TUI_LATIN_CAPITAL_LIGATURE_AE                          0x00c6
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX               0x00f4
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_DIAERESIS                0x00f6
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_GRAVE                    0x00f2
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX               0x00fb
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_GRAVE                    0x00f9
#define RLHK_TUI_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS                0x00ff
#define RLHK_TUI_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS              0x00d6
#define RLHK_TUI_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS              0x00dc
#define RLHK_TUI_CENT_SIGN                                          0x00a2
#define RLHK_TUI_POUND_SIGN                                         0x00a3
#define RLHK_TUI_YEN_SIGN                                           0x00a5
#define RLHK_TUI_PESETA_SIGN                                        0x20a7
#define RLHK_TUI_LATIN_SMALL_LETTER_F_WITH_HOOK                     0x0192
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_ACUTE                    0x00e1
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_ACUTE                    0x00ed
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_ACUTE                    0x00f3
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_ACUTE                    0x00fa
#define RLHK_TUI_LATIN_SMALL_LETTER_N_WITH_TILDE                    0x00f1
#define RLHK_TUI_LATIN_CAPITAL_LETTER_N_WITH_TILDE                  0x00d1
#define RLHK_TUI_FEMININE_ORDINAL_INDICATOR                         0x00aa
#define RLHK_TUI_MASCULINE_ORDINAL_INDICATOR                        0x00ba
#define RLHK_TUI_INVERTED_QUESTION_MARK                             0x00bf
#define RLHK_TUI_REVERSED_NOT_SIGN                                  0x2310
#define RLHK_TUI_NOT_SIGN                                           0x00ac
#define RLHK_TUI_VULGAR_FRACTION_ONE_HALF                           0x00bd
#define RLHK_TUI_VULGAR_FRACTION_ONE_QUARTER                        0x00bc
#define RLHK_TUI_INVERTED_EXCLAMATION_MARK                          0x00a1
#define RLHK_TUI_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK          0x00ab
#define RLHK_TUI_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK         0x00bb
#define RLHK_TUI_LIGHT_SHADE                                        0x2591
#define RLHK_TUI_MEDIUM_SHADE                                       0x2592
#define RLHK_TUI_DARK_SHADE                                         0x2593
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL                        0x2502
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT               0x2524
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE       0x2561
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE       0x2562
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE           0x2556
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE           0x2555
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT              0x2563
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL                       0x2551
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT                  0x2557
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT                    0x255d
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE             0x255c
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE             0x255b
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT                   0x2510
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT                    0x2514
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL               0x2534
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL             0x252c
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT              0x251c
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_HORIZONTAL                      0x2500
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL         0x253c
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE      0x255e
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE      0x255f
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT                   0x255a
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT                 0x2554
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL              0x2569
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL            0x2566
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT             0x2560
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_HORIZONTAL                     0x2550
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL        0x256c
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE       0x2567
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE       0x2568
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE     0x2564
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE     0x2565
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE            0x2559
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE            0x2558
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE          0x2552
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE          0x2553
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE 0x256b
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE 0x256a
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_LEFT                     0x2518
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT                  0x250c
#define RLHK_TUI_FULL_BLOCK                                         0x2588
#define RLHK_TUI_LOWER_HALF_BLOCK                                   0x2584
#define RLHK_TUI_LEFT_HALF_BLOCK                                    0x258c
#define RLHK_TUI_RIGHT_HALF_BLOCK                                   0x2590
#define RLHK_TUI_UPPER_HALF_BLOCK                                   0x2580
#define RLHK_TUI_GREEK_SMALL_LETTER_ALPHA                           0x03b1
#define RLHK_TUI_LATIN_SMALL_LETTER_SHARP_S                         0x00df
#define RLHK_TUI_GREEK_CAPITAL_LETTER_GAMMA                         0x0393
#define RLHK_TUI_GREEK_SMALL_LETTER_PI                              0x03c0
#define RLHK_TUI_GREEK_CAPITAL_LETTER_SIGMA                         0x03a3
#define RLHK_TUI_GREEK_SMALL_LETTER_SIGMA                           0x03c3
#define RLHK_TUI_MICRO_SIGN                                         0x00b5
#define RLHK_TUI_GREEK_SMALL_LETTER_TAU                             0x03c4
#define RLHK_TUI_GREEK_CAPITAL_LETTER_PHI                           0x03a6
#define RLHK_TUI_GREEK_CAPITAL_LETTER_THETA                         0x0398
#define RLHK_TUI_GREEK_CAPITAL_LETTER_OMEGA                         0x03a9
#define RLHK_TUI_GREEK_SMALL_LETTER_DELTA                           0x03b4
#define RLHK_TUI_INFINITY                                           0x221e
#define RLHK_TUI_GREEK_SMALL_LETTER_PHI                             0x03c6
#define RLHK_TUI_GREEK_SMALL_LETTER_EPSILON                         0x03b5
#define RLHK_TUI_INTERSECTION                                       0x2229
#define RLHK_TUI_IDENTICAL_TO                                       0x2261
#define RLHK_TUI_PLUS_MINUS_SIGN                                    0x00b1
#define RLHK_TUI_GREATER_THAN_OR_EQUAL_TO                           0x2265
#define RLHK_TUI_LESS_THAN_OR_EQUAL_TO                              0x2264
#define RLHK_TUI_TOP_HALF_INTEGRAL                                  0x2320
#define RLHK_TUI_BOTTOM_HALF_INTEGRAL                               0x2321
#define RLHK_TUI_DIVISION_SIGN                                      0x00f7
#define RLHK_TUI_ALMOST_EQUAL_TO                                    0x2248
#define RLHK_TUI_DEGREE_SIGN                                        0x00b0
#define RLHK_TUI_BULLET_OPERATOR                                    0x2219
#define RLHK_TUI_MIDDLE_DOT                                         0x00b7
#define RLHK_TUI_SQUARE_ROOT                                        0x221a
#define RLHK_TUI_SUPERSCRIPT_LATIN_SMALL_LETTER_N                   0x207f
#define RLHK_TUI_SUPERSCRIPT_TWO                                    0x00b2
#define RLHK_TUI_BLACK_SQUARE                                       0x25a0
#elif defined(__MSDOS__)
#define RLHK_TUI_SPACE                                              0x20
#define RLHK_TUI_EXCLAMATION_MARK                                   0x21
#define RLHK_TUI_QUOTATION_MARK                                     0x22
#define RLHK_TUI_NUMBER_SIGN                                        0x23
#define RLHK_TUI_DOLLAR_SIGN                                        0x24
#define RLHK_TUI_PERCENT_SIGN                                       0x25
#define RLHK_TUI_AMPERSAND                                          0x26
#define RLHK_TUI_APOSTROPHE                                         0x27
#define RLHK_TUI_LEFT_PARENTHESIS                                   0x28
#define RLHK_TUI_RIGHT_PARENTHESIS                                  0x29
#define RLHK_TUI_ASTERISK                                           0x2a
#define RLHK_TUI_PLUS_SIGN                                          0x2b
#define RLHK_TUI_COMMA                                              0x2c
#define RLHK_TUI_HYPHEN_MINUS                                       0x2d
#define RLHK_TUI_FULL_STOP                                          0x2e
#define RLHK_TUI_SOLIDUS                                            0x2f
#define RLHK_TUI_DIGIT_ZERO                                         0x30
#define RLHK_TUI_DIGIT_ONE                                          0x31
#define RLHK_TUI_DIGIT_TWO                                          0x32
#define RLHK_TUI_DIGIT_THREE                                        0x33
#define RLHK_TUI_DIGIT_FOUR                                         0x34
#define RLHK_TUI_DIGIT_FIVE                                         0x35
#define RLHK_TUI_DIGIT_SIX                                          0x36
#define RLHK_TUI_DIGIT_SEVEN                                        0x37
#define RLHK_TUI_DIGIT_EIGHT                                        0x38
#define RLHK_TUI_DIGIT_NINE                                         0x39
#define RLHK_TUI_COLON                                              0x3a
#define RLHK_TUI_SEMICOLON                                          0x3b
#define RLHK_TUI_LESS_THAN_SIGN                                     0x3c
#define RLHK_TUI_EQUALS_SIGN                                        0x3d
#define RLHK_TUI_GREATER_THAN_SIGN                                  0x3e
#define RLHK_TUI_QUESTION_MARK                                      0x3f
#define RLHK_TUI_COMMERCIAL_AT                                      0x40
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A                             0x41
#define RLHK_TUI_LATIN_CAPITAL_LETTER_B                             0x42
#define RLHK_TUI_LATIN_CAPITAL_LETTER_C                             0x43
#define RLHK_TUI_LATIN_CAPITAL_LETTER_D                             0x44
#define RLHK_TUI_LATIN_CAPITAL_LETTER_E                             0x45
#define RLHK_TUI_LATIN_CAPITAL_LETTER_F                             0x46
#define RLHK_TUI_LATIN_CAPITAL_LETTER_G                             0x47
#define RLHK_TUI_LATIN_CAPITAL_LETTER_H                             0x48
#define RLHK_TUI_LATIN_CAPITAL_LETTER_I                             0x49
#define RLHK_TUI_LATIN_CAPITAL_LETTER_J                             0x4a
#define RLHK_TUI_LATIN_CAPITAL_LETTER_K                             0x4b
#define RLHK_TUI_LATIN_CAPITAL_LETTER_L                             0x4c
#define RLHK_TUI_LATIN_CAPITAL_LETTER_M                             0x4d
#define RLHK_TUI_LATIN_CAPITAL_LETTER_N                             0x4e
#define RLHK_TUI_LATIN_CAPITAL_LETTER_O                             0x4f
#define RLHK_TUI_LATIN_CAPITAL_LETTER_P                             0x50
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Q                             0x51
#define RLHK_TUI_LATIN_CAPITAL_LETTER_R                             0x52
#define RLHK_TUI_LATIN_CAPITAL_LETTER_S                             0x53
#define RLHK_TUI_LATIN_CAPITAL_LETTER_T                             0x54
#define RLHK_TUI_LATIN_CAPITAL_LETTER_U                             0x55
#define RLHK_TUI_LATIN_CAPITAL_LETTER_V                             0x56
#define RLHK_TUI_LATIN_CAPITAL_LETTER_W                             0x57
#define RLHK_TUI_LATIN_CAPITAL_LETTER_X                             0x58
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Y                             0x59
#define RLHK_TUI_LATIN_CAPITAL_LETTER_Z                             0x5a
#define RLHK_TUI_LEFT_SQUARE_BRACKET                                0x5b
#define RLHK_TUI_REVERSE_SOLIDUS                                    0x5c
#define RLHK_TUI_RIGHT_SQUARE_BRACKET                               0x5d
#define RLHK_TUI_CIRCUMFLEX_ACCENT                                  0x5e
#define RLHK_TUI_LOW_LINE                                           0x5f
#define RLHK_TUI_GRAVE_ACCENT                                       0x60
#define RLHK_TUI_LATIN_SMALL_LETTER_A                               0x61
#define RLHK_TUI_LATIN_SMALL_LETTER_B                               0x62
#define RLHK_TUI_LATIN_SMALL_LETTER_C                               0x63
#define RLHK_TUI_LATIN_SMALL_LETTER_D                               0x64
#define RLHK_TUI_LATIN_SMALL_LETTER_E                               0x65
#define RLHK_TUI_LATIN_SMALL_LETTER_F                               0x66
#define RLHK_TUI_LATIN_SMALL_LETTER_G                               0x67
#define RLHK_TUI_LATIN_SMALL_LETTER_H                               0x68
#define RLHK_TUI_LATIN_SMALL_LETTER_I                               0x69
#define RLHK_TUI_LATIN_SMALL_LETTER_J                               0x6a
#define RLHK_TUI_LATIN_SMALL_LETTER_K                               0x6b
#define RLHK_TUI_LATIN_SMALL_LETTER_L                               0x6c
#define RLHK_TUI_LATIN_SMALL_LETTER_M                               0x6d
#define RLHK_TUI_LATIN_SMALL_LETTER_N                               0x6e
#define RLHK_TUI_LATIN_SMALL_LETTER_O                               0x6f
#define RLHK_TUI_LATIN_SMALL_LETTER_P                               0x70
#define RLHK_TUI_LATIN_SMALL_LETTER_Q                               0x71
#define RLHK_TUI_LATIN_SMALL_LETTER_R                               0x72
#define RLHK_TUI_LATIN_SMALL_LETTER_S                               0x73
#define RLHK_TUI_LATIN_SMALL_LETTER_T                               0x74
#define RLHK_TUI_LATIN_SMALL_LETTER_U                               0x75
#define RLHK_TUI_LATIN_SMALL_LETTER_V                               0x76
#define RLHK_TUI_LATIN_SMALL_LETTER_W                               0x77
#define RLHK_TUI_LATIN_SMALL_LETTER_X                               0x78
#define RLHK_TUI_LATIN_SMALL_LETTER_Y                               0x79
#define RLHK_TUI_LATIN_SMALL_LETTER_Z                               0x7a
#define RLHK_TUI_LEFT_CURLY_BRACKET                                 0x7b
#define RLHK_TUI_VERTICAL_LINE                                      0x7c
#define RLHK_TUI_RIGHT_CURLY_BRACKET                                0x7d
#define RLHK_TUI_TILDE                                              0x7e
#define RLHK_TUI_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA                0x80
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_DIAERESIS                0x81
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_ACUTE                    0x82
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX               0x83
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_DIAERESIS                0x84
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_GRAVE                    0x85
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE               0x86
#define RLHK_TUI_LATIN_SMALL_LETTER_C_WITH_CEDILLA                  0x87
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX               0x88
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_DIAERESIS                0x89
#define RLHK_TUI_LATIN_SMALL_LETTER_E_WITH_GRAVE                    0x8a
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_DIAERESIS                0x8b
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX               0x8c
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_GRAVE                    0x8d
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS              0x8e
#define RLHK_TUI_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE             0x8f
#define RLHK_TUI_LATIN_CAPITAL_LETTER_E_WITH_ACUTE                  0x90
#define RLHK_TUI_LATIN_SMALL_LIGATURE_AE                            0x91
#define RLHK_TUI_LATIN_CAPITAL_LIGATURE_AE                          0x92
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX               0x93
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_DIAERESIS                0x94
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_GRAVE                    0x95
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX               0x96
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_GRAVE                    0x97
#define RLHK_TUI_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS                0x98
#define RLHK_TUI_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS              0x99
#define RLHK_TUI_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS              0x9a
#define RLHK_TUI_CENT_SIGN                                          0x9b
#define RLHK_TUI_POUND_SIGN                                         0x9c
#define RLHK_TUI_YEN_SIGN                                           0x9d
#define RLHK_TUI_PESETA_SIGN                                        0x9e
#define RLHK_TUI_LATIN_SMALL_LETTER_F_WITH_HOOK                     0x9f
#define RLHK_TUI_LATIN_SMALL_LETTER_A_WITH_ACUTE                    0xa0
#define RLHK_TUI_LATIN_SMALL_LETTER_I_WITH_ACUTE                    0xa1
#define RLHK_TUI_LATIN_SMALL_LETTER_O_WITH_ACUTE                    0xa2
#define RLHK_TUI_LATIN_SMALL_LETTER_U_WITH_ACUTE                    0xa3
#define RLHK_TUI_LATIN_SMALL_LETTER_N_WITH_TILDE                    0xa4
#define RLHK_TUI_LATIN_CAPITAL_LETTER_N_WITH_TILDE                  0xa5
#define RLHK_TUI_FEMININE_ORDINAL_INDICATOR                         0xa6
#define RLHK_TUI_MASCULINE_ORDINAL_INDICATOR                        0xa7
#define RLHK_TUI_INVERTED_QUESTION_MARK                             0xa8
#define RLHK_TUI_REVERSED_NOT_SIGN                                  0xa9
#define RLHK_TUI_NOT_SIGN                                           0xaa
#define RLHK_TUI_VULGAR_FRACTION_ONE_HALF                           0xab
#define RLHK_TUI_VULGAR_FRACTION_ONE_QUARTER                        0xac
#define RLHK_TUI_INVERTED_EXCLAMATION_MARK                          0xad
#define RLHK_TUI_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK          0xae
#define RLHK_TUI_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK         0xaf
#define RLHK_TUI_LIGHT_SHADE                                        0xb0
#define RLHK_TUI_MEDIUM_SHADE                                       0xb1
#define RLHK_TUI_DARK_SHADE                                         0xb2
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL                        0xb3
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT               0xb4
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE       0xb5
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE       0xb6
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE           0xb7
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE           0xb8
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT              0xb9
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL                       0xba
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT                  0xbb
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT                    0xbc
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE             0xbd
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE             0xbe
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT                   0xbf
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT                    0xc0
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL               0xc1
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL             0xc2
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT              0xc3
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_HORIZONTAL                      0xc4
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL         0xc5
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE      0xc6
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE      0xc7
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT                   0xc8
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT                 0xc9
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL              0xca
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL            0xcb
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT             0xcc
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_HORIZONTAL                     0xcd
#define RLHK_TUI_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL        0xce
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE       0xcf
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE       0xd0
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE     0xd1
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE     0xd2
#define RLHK_TUI_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE            0xd3
#define RLHK_TUI_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE            0xd4
#define RLHK_TUI_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE          0xd5
#define RLHK_TUI_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE          0xd6
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE 0xd7
#define RLHK_TUI_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE 0xd8
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_UP_AND_LEFT                     0xd9
#define RLHK_TUI_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT                  0xda
#define RLHK_TUI_FULL_BLOCK                                         0xdb
#define RLHK_TUI_LOWER_HALF_BLOCK                                   0xdc
#define RLHK_TUI_LEFT_HALF_BLOCK                                    0xdd
#define RLHK_TUI_RIGHT_HALF_BLOCK                                   0xde
#define RLHK_TUI_UPPER_HALF_BLOCK                                   0xdf
#define RLHK_TUI_GREEK_SMALL_LETTER_ALPHA                           0xe0
#define RLHK_TUI_LATIN_SMALL_LETTER_SHARP_S                         0xe1
#define RLHK_TUI_GREEK_CAPITAL_LETTER_GAMMA                         0xe2
#define RLHK_TUI_GREEK_SMALL_LETTER_PI                              0xe3
#define RLHK_TUI_GREEK_CAPITAL_LETTER_SIGMA                         0xe4
#define RLHK_TUI_GREEK_SMALL_LETTER_SIGMA                           0xe5
#define RLHK_TUI_MICRO_SIGN                                         0xe6
#define RLHK_TUI_GREEK_SMALL_LETTER_TAU                             0xe7
#define RLHK_TUI_GREEK_CAPITAL_LETTER_PHI                           0xe8
#define RLHK_TUI_GREEK_CAPITAL_LETTER_THETA                         0xe9
#define RLHK_TUI_GREEK_CAPITAL_LETTER_OMEGA                         0xea
#define RLHK_TUI_GREEK_SMALL_LETTER_DELTA                           0xeb
#define RLHK_TUI_INFINITY                                           0xec
#define RLHK_TUI_GREEK_SMALL_LETTER_PHI                             0xed
#define RLHK_TUI_GREEK_SMALL_LETTER_EPSILON                         0xee
#define RLHK_TUI_INTERSECTION                                       0xef
#define RLHK_TUI_IDENTICAL_TO                                       0xf0
#define RLHK_TUI_PLUS_MINUS_SIGN                                    0xf1
#define RLHK_TUI_GREATER_THAN_OR_EQUAL_TO                           0xf2
#define RLHK_TUI_LESS_THAN_OR_EQUAL_TO                              0xf3
#define RLHK_TUI_TOP_HALF_INTEGRAL                                  0xf4
#define RLHK_TUI_BOTTOM_HALF_INTEGRAL                               0xf5
#define RLHK_TUI_DIVISION_SIGN                                      0xf6
#define RLHK_TUI_ALMOST_EQUAL_TO                                    0xf7
#define RLHK_TUI_DEGREE_SIGN                                        0xf8
#define RLHK_TUI_BULLET_OPERATOR                                    0xf9
#define RLHK_TUI_MIDDLE_DOT                                         0xfa
#define RLHK_TUI_SQUARE_ROOT                                        0xfb
#define RLHK_TUI_SUPERSCRIPT_LATIN_SMALL_LETTER_N                   0xfc
#define RLHK_TUI_SUPERSCRIPT_TWO                                    0xfd
#define RLHK_TUI_BLACK_SQUARE                                       0xfe
#endif /* __MSDOS__ */

/* Implementation */
#ifdef RLHK_TUI_IMPLEMENTATION

static int rlhk_tui_width;
static int rlhk_tui_height;

#if (defined(__unix__) || defined(__APPLE__)) && !defined(__DJGPP__)
/* Here we're free to assume an int is 32-bits. */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

struct termios rlhk_tui_termios_orig;

/* The last written display. */
static unsigned rlhk_tui_oldc[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
static unsigned char rlhk_tui_olda[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];

/* Characters to be written on the next flush. */
static unsigned rlhk_tui_bufc[RLHK_TUI_MAX_HEIGHT][RLHK_TUI_MAX_WIDTH];
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

RLHK_TUI_API
void
rlhk_tui_flush(void)
{
    int x, y;
    /* Estimated worst-case buffer. */
    static unsigned char buf[RLHK_TUI_MAX_HEIGHT * RLHK_TUI_MAX_WIDTH *
                             (5 + 11 + 3)];
    unsigned char *p = buf;
    unsigned last_a = -1u;
    int cx = -1;
    int cy = -1;
    for (y = 0; y < rlhk_tui_height; y++) {
        for (x = 0; x < rlhk_tui_width; x++) {
            unsigned *oc = &rlhk_tui_oldc[y][x];
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
                    int fg = (a & 0x07) + 30;
                    int bg = ((a >> 4) & 0x07) + (a & 0x80 ? 100 : 40);
                    *p++ = 0x1b;
                    *p++ = '[';
                    if (a & 0x08) {
                        *p++ = '1';
                        *p++ = ';';
                    }
                    p = rlhk_tui_itoa(p, fg);
                    *p++ = ';';
                    p = rlhk_tui_itoa(p, bg);
                    *p++ = 'm';
                    last_a = a;
                }
                cx = x + 1;
                cy = y;
                *oc = c;
                *oa = a;
                do {
                    *p++ = c;
                    c >>= 8;
                } while (c);
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

#elif defined(__DJGPP__)
#ifdef __STRICT_ANSI__
#  undef __STRICT_ANSI__  /* Disable DJGPP annoyance. */
#endif
#include <conio.h>
#include <sys/nearptr.h>

static unsigned short rlhk_tui_buf[25][80];

RLHK_TUI_API
int
rlhk_tui_init(int width, int height)
{
    rlhk_tui_width = width;
    rlhk_tui_height = height;
    return width <= 80 && height <= 25;
}

RLHK_TUI_API
void
rlhk_tui_release(void)
{
    memset(rlhk_tui_buf, 0, sizeof(rlhk_tui_buf));
}

RLHK_TUI_API
void
rlhk_tui_putc(int x, int y, unsigned c, unsigned attr)
{
    unsigned a = (attr & 0x2a) | ((attr >> 2) & 0x11) | ((attr << 2) & 0x44);
    rlhk_tui_buf[y][x] = c | (a << 8);
}

RLHK_TUI_API
void
rlhk_tui_flush(void)
{
    char *screen = (char *)0xb8000 + __djgpp_conventional_base;
    __djgpp_nearptr_enable();
    memcpy(screen, rlhk_tui_buf, sizeof(rlhk_tui_buf));
    __djgpp_nearptr_disable();
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
rlhk_tui_title(const char *s)
{
    (void)s;
    /* empty */
}

RLHK_TUI_API
void
rlhk_tui_size(int *width, int *height)
{
    *width = 80;
    *height = 25;
}

#endif /* __MSDOS__ */
#endif /* RLHK_TUI_IMPLEMENTATION */
#endif /* RLHK_TUI_H */
