#include "backend.h"

/**
 * @brief Массивы всех использованных фигур в тетрисе.
 */
static const BrickBlock FIGURES[7][5][5] = {
    ///<  O Figure
    {{{0}, {0}, {0}, {0}, {0}},
     {{0}, {1}, {1}, {0}, {0}},
     {{0}, {1}, {1}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}}},

    ///<  I Figure
    {{{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}}},

    ///<  T Figure
    {{{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {1}, {1}, {1}, {0}},
     {{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}}},

    ///<  S Figure
    {{{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {1}, {1}, {0}},
     {{0}, {1}, {1}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}}},

    ///<  Z Figure
    {{{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}},
     {{0}, {1}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {1}, {0}},
     {{0}, {0}, {0}, {0}, {0}}},

    ///<  J Figure
    {{{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {1}, {1}, {0}, {0}},
     {{0}, {0}, {0}, {0}, {0}}},

    ///< L Figure
    {{{0}, {0}, {0}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {0}, {0}},
     {{0}, {0}, {1}, {1}, {0}},
     {{0}, {0}, {0}, {0}, {0}}}};

/**
 * @brief Функция получения фигуры по индексу.
 */
const BrickBlock (*get_figure(int index))[5] {
  if (index < 0 || index >= 7) return NULL;
  return FIGURES[index];
}

/**
 * @brief Функция создает и возвращает динамический массив указателей на фигуры.
 */
BrickBlock **create_templates() {
  BrickBlock **Brick_templates = malloc(7 * sizeof(BrickBlock *));
  if (!Brick_templates) return NULL;  ///< Проверяем выделение памяти
  for (int i = 0; i < 7; i++) {
    Brick_templates[i] = (BrickBlock *)FIGURES[i];
  }
  return Brick_templates;
}

/**
 * @brief Чистим память от использованных фигур.
 */
void free_templates(BrickBlock **templates) {
  if (templates) free(templates);
}

/**
 * @brief Информация о статусе игры.
 */
Game *Brickg;

/**
 * @brief Обновляет информацию о текущем состоянии игры,
 * игровое поле, следующую фигуру, счет, рекорд, уровень и скорость.
 */
GameInfo_t updatecurrentstate() {
  GameInfo_t game_info = {0};
  calculate(Brickg);
  if (Brickg->state != GAMEOVER) {  ///< так нет Seg при q, но и не выходит
    if (game_info.field != NULL)
      free_print_field(game_info.field, Brickg->field->height);
    game_info.field =
        create_print_field(Brickg->field->width, Brickg->field->height);
    if (game_info.next != NULL)
      free_next_block(game_info.next, Brickg->form->size);
    game_info.next = create_next_block(Brickg->form->size);
    game_info.score = Brickg->score;
    game_info.high_score = Brickg->high_score;
    game_info.level = Brickg->level;
    game_info.speed = Brickg->speed;
    game_info.pause = Brickg->pause;
  }
  return game_info;
}

/**
 * @brief Инициализирует новую игру, создавая игровое поле заданной ширины и
 * высоты, устанавливает случайную начальную фигуру и устанавливая начальные
 * значения для счета, уровня и скорости.
 */
Game *create_game(int field_width, int field_height, int figures_size,
                  int count) {
  Game *Brickg = (Game *)malloc(sizeof(Game));
  Brickg->field = create_field(field_width, field_height);
  Brickg->Brick_templates = create_templates();
  Brickg->form = create_form(count, figures_size, Brickg->Brick_templates);
  Brickg->score = 0;
  Brickg->high_score = load_score();
  Brickg->ticks = 30;
  Brickg->ticks_left = 30;
  Brickg->speed = 1;
  Brickg->level = 1;
  Brickg->pause = 1;
  Brickg->state = INIT;
  Brickg->next = rand() % Brickg->form->count;
  return Brickg;
}

/**
 * @brief Запускает игру, создавая экземпляр игры и начальную фигуру.
 */
void init_game() {
  Brickg = create_game(10, 20, 5, 7);
  BrickPlayer *player = (BrickPlayer *)malloc(sizeof(BrickPlayer));
  player->action = Start;
  Brickg->player = player;
  drop_new_figure(Brickg);
}

/**
 * @brief Создает игровое поле заданной ширины и высоты.
 */
BrickField *create_field(int width, int height) {
  BrickField *Brickf = (BrickField *)malloc(sizeof(BrickField));
  Brickf->width = width;
  Brickf->height = height;
  Brickf->blocks = (BrickBlock **)malloc(sizeof(BrickBlock *) * height);
  for (int i = 0; i < height; i++) {
    Brickf->blocks[i] = (BrickBlock *)malloc(sizeof(BrickBlock) * width);
    for (int j = 0; j < width; j++) {
      Brickf->blocks[i][j].b = 0;
    }
  }
  return Brickf;
}

/**
 * @brief Освобождает память, занятую игровым полем.
 */
void free_field(BrickField *Brickf) {
  if (Brickf) {
    for (int i = 0; i < Brickf->height; i++) free(Brickf->blocks[i]);
    free(Brickf->blocks);
    free(Brickf);
  }
}

/**
 * @brief Создает структуру, содержащую список фигур, используемых в игре.
 */
BrickForm *create_form(int count, int figures_size,
                       BrickBlock **figures_template) {
  BrickForm *Brickft = (BrickForm *)malloc(sizeof(BrickForm));
  Brickft->count = count;
  Brickft->size = figures_size;
  Brickft->blocks = figures_template;
  return Brickft;
}

/**
 * @brief Освобождает память, занятую структурой с фигурами.
 */
void free_form(BrickForm *Brickft) {
  if (Brickft) free(Brickft);
}

/**
 * @brief Выделяяет память под новую фигуру и инициализируя ее размер.
 */
BrickFigure *create_figure(Game *Brickg) {
  BrickFigure *figure = (BrickFigure *)malloc(sizeof(BrickFigure));
  figure->x = 0;
  figure->y = 0;
  figure->size = Brickg->form->size;
  figure->blocks = (BrickBlock **)malloc(sizeof(BrickBlock *) * figure->size);
  for (int i = 0; i < figure->size; i++) {
    figure->blocks[i] = (BrickBlock *)malloc(sizeof(BrickBlock) * figure->size);
    for (int j = 0; j < figure->size; j++) {
      figure->blocks[i][j].b = 0;
    }
  }
  return figure;
}

/**
 * @brief Освобождает память, занятую фигурой.
 */
void free_figure(BrickFigure *tf) {
  if (tf) {
    if (tf->blocks) {
      for (int i = 0; i < tf->size; i++) {
        if (tf->blocks[i]) {
          free(tf->blocks[i]);
        }
      }
      free(tf->blocks);
    }
    free(tf);
  }
}

/**
 * @brief Создает массив для отображения игрового поля,
 * @param sym означает наличие/отсутствие блока.
 */
int **create_print_field(int width, int height) {
  int **print_field = (int **)malloc(height * sizeof(int *));
  for (int i = 0; i < height; i++) {
    print_field[i] = (int *)malloc(width * sizeof(int));
  }
  BrickField *field = Brickg->field;
  BrickFigure *figure = Brickg->figure;
  for (int i = 0; i < field->height; i++) {
    for (int j = 0; j < field->width; j++) {
      int sym = 0;
      if (field->blocks[i][j].b != 0)
        sym = 1;
      else {
        int x = j - figure->x;
        int y = i - figure->y;
        if (x >= 0 && x < figure->size && y >= 0 && y < figure->size)
          if (figure->blocks[y][x].b != 0) sym = 1;
      }
      print_field[i][j] = sym;
    }
  }
  return print_field;
}

/**
 * @brief Чистим память поля.
 */
void free_print_field(int **print_field, int height) {
  if (print_field) {
    for (int i = 0; i < height; i++) {
      if (print_field[i]) free(print_field[i]);
    }
    free(print_field);
  }
}

/**
 * @brief Следующая фигура.
 */
int **create_next_block(int size) {
  int **next = (int **)malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++) {
    next[i] = (int *)malloc(size * sizeof(int));
    for (int j = 0; j < Brickg->form->size; j++) {
      next[i][j] =
          Brickg->Brick_templates[Brickg->next][i * Brickg->form->size + j].b;
    }
  }
  return next;
}

/**
 * @brief Чистим память от след. фигуры.
 */
void free_next_block(int **next, int size) {
  if (next) {
    for (int i = 0; i < size; i++) free(next[i]);
    free(next);
  }
}

/**
 * @brief Чистим память от поля и след. фиг.
 */
void free_gui(GameInfo_t game, int size, int height) {
  free_print_field(game.field, height);
  free_next_block(game.next, size);
}

/**
 * @brief  Функция полной очистки.
 */
void free_game(Game *Brickg) {
  if (Brickg) {
    if (Brickg->figure != NULL) free_figure(Brickg->figure);
    free_field(Brickg->field);
    free_form(Brickg->form);
    free_templates(Brickg->Brick_templates);
    free(Brickg->player);
    free(Brickg);
  }
}

/**
 * @brief Выбирает новую фигуру и помещаем на игровое поле.
 */
void drop_new_figure(Game *Brickg) {
  Brickg->figure = create_figure(Brickg);
  BrickFigure *figure = create_figure(Brickg);
  figure->x = Brickg->field->width / 2 - figure->size / 2;
  figure->y = 0;
  int fnum = Brickg->next;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      figure->blocks[i][j].b =
          Brickg->form->blocks[fnum][i * figure->size + j].b;
  if (Brickg->figure != NULL) free_figure(Brickg->figure);
  Brickg->figure = figure;
  fnum = rand() % Brickg->form->count;
  Brickg->next = fnum;
}

/**
 * @brief Нажатия кнопок.
 */
void userinput(UserAction_t action, bool hold) {
  if (!hold) {
    switch (action) {
      case Left:
        Brickg->player->action = Left;
        break;
      case Right:
        Brickg->player->action = Right;
        break;
      case Up:
        Brickg->player->action = Up;
        break;
      case Down:
        Brickg->player->action = Down;
        break;
      case Start:
        Brickg->player->action = Start;
        break;
      case Pause:
        Brickg->player->action = Pause;
        break;
      case Terminate:
        Brickg->player->action = Terminate;
        break;
      default:
        Brickg->player->action = Action;
        break;
    }
  }
}

/**
 * @brief Логика игры, обработка падения, столкновения и соприкосновения блоков.
 */
void calculate(Game *Brickg) {
  if (Brickg->ticks_left <= 0 && Brickg->state != PAUSE &&
      Brickg->state != INIT)
    calculate_Brick(Brickg);
  if (Brickg->state == GAMEOVER) return;
  switch (Brickg->player->action) {
    case Right:
      if (Brickg->pause) break;
      move_figure_right(Brickg);
      if (is_collision(Brickg)) move_figure_left(Brickg);
      break;
    case Left:
      if (Brickg->pause) break;
      move_figure_left(Brickg);
      if (is_collision(Brickg)) move_figure_right(Brickg);
      break;
      /*
          Вариант с падением фигуры по нажатию.
          case Down:
            if (Brickg->pause) break;
            while (!is_collision(Brickg)) {
              move_figure_down(Brickg);
            }
            move_figure_up(Brickg); // Вернуть наверх после столкновения
          break;
      */
    case Down:
      if (Brickg->pause) break;
      move_figure_down(Brickg);
      if (is_collision(Brickg)) move_figure_up(Brickg);
      break;
    case Up: {
      if (Brickg->pause) break;
      handle_rotation(Brickg);
    } break;
    case Pause:
      if (Brickg->pause == 1) {
        Brickg->pause = 0;
        Brickg->state = MOVING;
      } else {
        Brickg->pause = 1;
        Brickg->state = PAUSE;
      }
      break;
    case Terminate:
      Brickg->state = GAMEOVER;  ///< закрытие игры в любой момент
      Brickg->pause = -1;
      break;
    case Start:
      Brickg->pause = 0;
      Brickg->state = MOVING;
      break;
    default:
      break;
  }
  Brickg->ticks_left--;
}

/**
 * @brief Подготовка фигуры к падению.
 */
void calculate_Brick(Game *Brickg) {
  Brickg->ticks_left = Brickg->ticks;
  move_figure_down(Brickg);
  Brickg->state = MOVING;
  if (is_collision(Brickg)) {
    move_figure_up(Brickg);
    plant_figure(Brickg);
    score(Brickg);
    if (Brickg->figure != NULL) free_figure(Brickg->figure);
    drop_new_figure(Brickg);
    Brickg->state = DROP;
    if (is_collision(Brickg)) {  // игра окончена
      Brickg->state = GAMEOVER;
    }
  }
}

/**
 * @brief Перемещение.
 */
void move_figure_down(Game *Brickg) { Brickg->figure->y++; }
void move_figure_up(Game *Brickg) { Brickg->figure->y--; }
void move_figure_right(Game *Brickg) { Brickg->figure->x++; }
void move_figure_left(Game *Brickg) { Brickg->figure->x--; }

/**
 * @brief Проверка соприкосновения с игровым полем.
 */
int is_collision(Game *Brickg) {
  BrickFigure *figure = Brickg->figure;
  BrickField *field = Brickg->field;
  int collision = 0;

  for (int i = 0; i < figure->size && !collision; i++) {
    for (int j = 0; j < figure->size && !collision; j++) {
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx < 0 || fx >= field->width || fy < 0 || fy >= field->height ||
            field->blocks[fy][fx].b != 0) {
          Brickg->state = COLLISION;
          collision = 1;
        }
      }
    }
  }
  return collision;
}

/**
 * @brief Логика чистки заполненных строк блоками.
 */
int erase_lines(Game *Brickg) {
  BrickField *tfl = Brickg->field;
  int count = 0;
  for (int i = tfl->height - 1; i >= 0; i--) {
    while (line_filled(i, tfl)) {
      drop_line(i, tfl);
      count++;
    }
  }
  return count;
}

/**
 * @brief Проверка заполненности строки.
 */
int line_filled(int i, BrickField *tfl) {
  int res = 1;
  for (int j = 0; j < tfl->width; j++)
    if (tfl->blocks[i][j].b == 0) res = 0;
  return res;
}

/**
 * @brief Удаление строки.
 */
void drop_line(int i, BrickField *tfl) {
  if (i == 0)
    for (int j = 0; j < tfl->width; j++) tfl->blocks[i][j].b = 0;
  else {
    for (int k = i; k > 0; k--)
      for (int j = 0; j < tfl->width; j++)
        tfl->blocks[k][j].b = tfl->blocks[k - 1][j].b;
  }
}

/**
 * @brief Механика вращения.
 */
BrickFigure *turn_figure(Game *Brickg) {
  BrickFigure *figure = create_figure(Brickg);
  BrickFigure *old_figure = Brickg->figure;
  figure->x = old_figure->x;
  figure->y = old_figure->y;
  int size = figure->size;
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      figure->blocks[i][j].b = old_figure->blocks[j][size - 1 - i].b;
  return figure;
}

/**
 * @brief Вращение возможно ли, если есть столкновение.
 */
void handle_rotation(Game *Brickg) {
  BrickFigure *t = turn_figure(Brickg);
  BrickFigure *tOld = Brickg->figure;
  Brickg->figure = t;
  if (is_collision(Brickg)) {
    Brickg->figure = tOld;
    free_figure(t);
  } else
    free_figure(tOld);
}

/**
 * @brief Фиксация фигуры в случае окончания ее движения.
 */
void plant_figure(Game *Brickg) {
  BrickFigure *figure = Brickg->figure;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx >= 0 && fx < Brickg->field->width && fy >= 0 &&
            fy < Brickg->field->height) {
          Brickg->field->blocks[fy][fx].b = figure->blocks[i][j].b;
        }
      }
}

/**
 * @brief Начисление, но не пенсионные.
 */
void score(Game *Brickg) {
  int erased_lines = erase_lines(Brickg);
  switch (erased_lines) {
    case 0:
      break;
    case 1:
      Brickg->score += 100;
      break;
    case 2:
      Brickg->score += 300;
      break;
    case 3:
      Brickg->score += 700;
      break;
    default:
      Brickg->score += 1500;
      break;
  }
  if (Brickg->score > Brickg->high_score) {
    Brickg->high_score = Brickg->score;
    save_score(Brickg->high_score);
  }
  int new_level = Brickg->score / 600 + 1;
  if (new_level > Brickg->level && new_level <= 10) {
    Brickg->level = new_level;
    Brickg->speed = new_level;
  }
}

/**
 * @brief Сохранение очков в файл.
 */
void save_score(int high_score) {
  FILE *file = fopen("max_score.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

/**
 * @brief Сохранение в файл и получение максимального счета.
 */
int load_score() {
  int high_score = 0;
  FILE *file = fopen("max_score.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}
