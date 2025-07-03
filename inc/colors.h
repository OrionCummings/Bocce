#ifndef __COLORS_H
#define __COLORS_H

#define COLOR_VSC_0 ((Color){10, 10, 15, 255})
#define COLOR_VSC_1 ((Color){30, 30, 30, 255})
#define COLOR_VSC_2 ((Color){37, 37, 38, 255})
#define COLOR_VSC_3 ((Color){45, 45, 48, 255})
#define COLOR_VSC_4 ((Color){62, 62, 66, 255})
#define COLOR_VSC_5 ((Color){0, 122, 204, 255})

#define COLOR_GAME_BACKGROUND ((Color){196, 177, 174, 255})
#define COLOR_GAME_INFO_BACKGROUND ((Color){202, 183, 162, 255})
#define COLOR_CHAT_BACKGROUND ((Color){133, 135, 134, 255})

#define COLOR_SHADOW (COLOR_VSC_0)
#define COLOR_BACKGROUND (COLOR_VSC_1)
#define COLOR_TEXT (COLOR_VSC_4)
#define COLOR_HIGHLIGHT (COLOR_VSC_5)

// https://coolors.co/palette/606c38-283618-fefae0-dda15e-bc6c25
#define COLOR_COURT_BASE_DARK ((Color){40, 54, 24, 255})
#define COLOR_COURT_BASE ((Color){96, 108, 56, 255})
#define COLOR_COURT_BEIGE ((Color){254, 250, 224, 255})
#define COLOR_COURT_WALL ((Color){221, 161, 94, 255})
#define COLOR_COURT_WALL_DARK ((Color){188, 108, 37, 255})

#define COLOR_RANDOM ((Color){(unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), 255})

#endif