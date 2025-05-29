#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/tetris/backend.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define START_X 5  ///< Смещение X
#define START_Y 2  ///< Смещение Y
#define INFO_X 30  ///< Начальная X
#define INFO_Y 2   ///< Начальная Y

void init_gui();
void printGame(GameInfo_t game, struct timespec sp_start,
               struct timespec sp_end);
void print_field(GameInfo_t game);
void print_next_figure(GameInfo_t game);
void print_info(GameInfo_t game);
void clear_lines();
UserAction_t get_action();
void handle_delay(struct timespec sp_start, struct timespec sp_end,
                  int game_speed);
void draw_border();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

#endif