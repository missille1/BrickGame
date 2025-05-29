#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"

int main() {
  struct timespec sp_start, sp_end = {0, 0};
  srand(time(NULL));
  init_gui();
  int running = 1;  ///< Флаг для управления выходом из игры
  while (running) {
    init_game();
    while (Brickg->state != GAMEOVER) {
      clock_gettime(CLOCK_MONOTONIC, &sp_start);
      UserAction_t action = get_action();
      if (action == Terminate) {  ///< Завершаем игру при нажатии 'q'
        running = 0;
        break;
      }
      userinput(action, 0);
      GameInfo_t game_info = updatecurrentstate();
      if (Brickg->state == GAMEOVER) {
        game_info.pause = -1;
        print_info(game_info);  ///< Выводим информацию о завершении игры
        refresh();    ///< Обновляем экран
        napms(2000);  ///< Делаем задержку на 4 секунды
        free_gui(game_info, Brickg->form->size,
                 Brickg->field->height);  ///< Очищаем GUI
      } else
        printGame(game_info, sp_start, sp_end);
    };
    free_game(Brickg);
  }
  endwin();
  return 0;
}