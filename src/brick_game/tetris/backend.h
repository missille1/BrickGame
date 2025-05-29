#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum { INIT, DROP, MOVING, COLLISION, PAUSE, GAMEOVER } GameState;

typedef struct {  ///< Структура с информацией об игре
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct BrickBlock {  ///< Структура для определения существования блока
                             ///< на поле
  int b;
} BrickBlock;

typedef struct BrickFigure {  ///< Определение позиции фигуры и ее размеры.
  int x;
  int y;
  int size;
  BrickBlock **blocks;
} BrickFigure;

typedef struct BrickForm {  ///< Все фигуры.
  int count;
  int size;
  BrickBlock **blocks;
} BrickForm;

typedef struct BrickField {  ///< Игровое поле, ширина высота, ссылки на блоки.
  int width;
  int height;
  BrickBlock **blocks;
} BrickField;

typedef struct BrickPlayer {  ///< Действия игрока.
  int action;
} BrickPlayer;

typedef struct Game {  ///< Объединяющая структура.
  BrickField *field;
  BrickFigure *figure;
  BrickForm *form;
  BrickPlayer *player;
  BrickBlock **Brick_templates;
  int score;
  int high_score;
  int ticks_left;
  int ticks;
  int speed;
  int level;
  int next;
  int pause;
  int state;
} Game;

extern Game *Brickg;  ///< Игровой процесс.
GameInfo_t updatecurrentstate();
Game *create_game(int field_width, int field_height, int figures_size,
                  int count);
void init_game();
void free_game(Game *Brickg);
void free_gui(GameInfo_t game, int size, int height);

BrickBlock **create_templates();  ///< Управление памятью.
void free_templates(BrickBlock **templates);
BrickField *create_field(int width, int height);
void free_field(BrickField *Brickf);
BrickForm *create_form(int count, int figures_size,
                       BrickBlock **figures_template);
void free_form(BrickForm *Brickft);
BrickFigure *create_figure(Game *Brickg);
void free_figure(BrickFigure *tf);
int **create_print_field(int width, int height);
void free_print_field(int **print_field, int height);
int **create_next_block(int size);
void free_next_block(int **next, int size);
void drop_new_figure(Game *Brickg);
void calculate(Game *Brickg);
void calculate_Brick(Game *Brickg);
void handle_rotation(Game *Brickg);

void move_figure_down(Game *Brickg);  ///< Движение фигур.
void move_figure_up(Game *Brickg);
void move_figure_right(Game *Brickg);
void move_figure_left(Game *Brickg);

int is_collision(Game *Brickg);  ///< Проверки и действия.
int erase_lines(Game *Brickg);
void plant_figure(Game *Brickg);
int line_filled(int i, BrickField *tfl);
void drop_line(int i, BrickField *tfl);

BrickFigure *turn_figure(Game *Brickg);  ///< Взаимодействие с игроком.
void userinput(UserAction_t action, bool hold);

void score(Game *Brickg);  ///< Управление очками.
void save_score(int high_score);
int load_score();

#endif