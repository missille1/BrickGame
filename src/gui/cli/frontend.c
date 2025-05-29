#include "frontend.h"

/**
 * @brief Инициализируем графику.
 */
void init_gui() {
  initscr();
  curs_set(0);
  start_color();
  init_color(COLOR_BLUE, 68, 71, 90);
  init_color(COLOR_BLACK, 40, 42, 54);
  init_pair(1, COLOR_WHITE, COLOR_BLUE);   ///< Фон
  init_pair(2, COLOR_WHITE, COLOR_WHITE);  //< Общий цвет
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
}

void printGame(GameInfo_t game, struct timespec sp_start,
               struct timespec sp_end) {
  print_field(game);
  draw_border();
  print_next_figure(game);
  print_info(game);
  free_gui(game, 5, 20);
  handle_delay(sp_start, sp_end, game.speed);
  refresh();
}

/**
 * @brief Логика рамки
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Рамка.
 */
void draw_border() {
  print_rectangle(1.5, FIELD_HEIGHT + 2, 4, 25);
  print_rectangle(1.5, FIELD_HEIGHT + 2, 28, 50);
}

void print_field(GameInfo_t game) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      int sym = game.field[i][j] != 0 ? 2 : 1;
      attron(COLOR_PAIR(sym));
      mvprintw(i + START_Y, j * 2 + START_X, " ");
      mvprintw(i + START_Y, j * 2 + START_X + 1, "  ");
      attroff(COLOR_PAIR(sym));
    }
  }
}

/**
 * @brief Следующая фигура
 */
void print_next_figure(GameInfo_t game) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      int sym = game.next[i][j] != 0 ? 2 : 0;
      attron(COLOR_PAIR(sym));
      mvaddch(i + 2, j * 2 + 31, ' ');
      mvaddch(i + 2, j * 2 + 32, ' ');
      attroff(COLOR_PAIR(sym));
    }
  }
}

/**
 * @brief Состояние
 */
void print_info(GameInfo_t game) {
  attron(COLOR_PAIR(4));
  clear_lines();  ///< Очищаем строки перед выводом новой информации
  mvwprintw(stdscr, INFO_Y, INFO_X, "Next figure:");
  mvwprintw(stdscr, INFO_Y + 5, INFO_X, "Score: %d", game.score);
  mvwprintw(stdscr, INFO_Y + 7, INFO_X, "High score: %d", game.high_score);
  mvwprintw(stdscr, INFO_Y + 9, INFO_X, "Lvl: %d", game.level);
  mvwprintw(stdscr, INFO_Y + 10, INFO_X, "Speed: %d", game.speed);
  mvwprintw(stdscr, INFO_Y + 16, INFO_X, "s - Start");
  mvwprintw(stdscr, INFO_Y + 17, INFO_X, "p - Pause");
  mvwprintw(stdscr, INFO_Y + 18, INFO_X, "arrows - Move");
  mvwprintw(stdscr, INFO_Y + 19, INFO_X, "space - Rotate");
  attroff(COLOR_PAIR(4));
  if (game.pause == 1) {
    mvwprintw(stdscr, INFO_Y + 13, INFO_X, "        ");
    mvwprintw(stdscr, INFO_Y + 13, INFO_X, "PAUSE");
  } else if (game.pause == -1) {
    mvwprintw(stdscr, INFO_Y + 13, INFO_X, "        ");
    mvwprintw(stdscr, INFO_Y + 13, INFO_X, "GAMEOVER");
  } else {
    mvwprintw(stdscr, INFO_Y + 13, INFO_X, "        ");
  }
}

/**
 * @brief Функция для очистки строк
 */
void clear_lines() {
  mvwprintw(stdscr, INFO_Y + 5, INFO_X,
            "Score:         ");  ///< Очистка строки для "Score"
  mvwprintw(stdscr, INFO_Y + 7, INFO_X,
            "High score:         ");  ///< Очистка строки для "High score"
  mvwprintw(stdscr, INFO_Y + 9, INFO_X,
            "Lvl:       ");  ///< Очистка строки для "Lvl"
  mvwprintw(stdscr, INFO_Y + 10, INFO_X,
            "Speed:       ");  ///< Очистка строки для "Speed"
  mvwprintw(stdscr, INFO_Y + 13, INFO_X, "        ");
}

/**
 * @brief Функция движения
 */
UserAction_t get_action() {
  UserAction_t res = Action;
  keypad(stdscr, TRUE);
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      res = Left;
      break;
    case KEY_RIGHT:
      res = Right;
      break;
    case ' ':
      res = Up;
      break;
    case KEY_DOWN:
      res = Down;
      break;
    case 's':
      res = Start;
      break;
    case 'p':
      res = Pause;
      break;
    case 'q':
      res = Terminate;
      break;
    default:
      res = Action;
      break;
  }
  return res;
}

/**
 * @brief Обрабатывает задержку между кадрами игры, чтобы контролировать
 * скорость падения фигур. Эта функция рассчитывает, сколько времени должно
 * пройти между кадрами, исходя из текущей скорости игры, и использует
 * nanosleep() для установки задержки, если необходимо.
 */
void handle_delay(struct timespec sp_start, struct timespec sp_end,
                  int game_speed) {
  clock_gettime(CLOCK_MONOTONIC, &sp_end);
  struct timespec ts1, ts2 = {0, 0};
  if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
      (ts2.tv_nsec = (20000000 - game_speed * 1500000) -
                     (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
    nanosleep(&ts2, &ts1);
  }
}