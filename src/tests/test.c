#include <check.h>

#include "../brick_game/tetris/backend.h"

START_TEST(test_score) {  ///< Очки.
  init_game();
  Brickg->pause = 0;
  Brickg->high_score = 0;
  Brickg->score = 700;
  int line_fill = 18;
  for (int i = 0; i < Brickg->field->width; i++) {
    Brickg->field->blocks[line_fill][i].b = 1;
  }
  score(Brickg);

  ck_assert_int_eq(Brickg->score, 800);
  free_game(Brickg);
}
END_TEST

START_TEST(test_score_multiple_lines) {
  init_game();
  Brickg->score = 0;
  for (int i = 0; i < Brickg->field->width;
       i++) {  ///< Создаем 2 заполненные линии
    Brickg->field->blocks[18][i].b = 1;
    Brickg->field->blocks[19][i].b = 1;
  }
  score(Brickg);
  ck_assert_int_eq(Brickg->score, 300);  ///< 2 линии = +300 очков
  free_game(Brickg);
}
END_TEST

START_TEST(test_score_four_lines) {
  init_game();
  Brickg->score = 0;
  for (int i = 0; i < Brickg->field->width; i++) {  ///< Заполняем 4 линии
    Brickg->field->blocks[16][i].b = 1;
    Brickg->field->blocks[17][i].b = 1;
    Brickg->field->blocks[18][i].b = 1;
    Brickg->field->blocks[19][i].b = 1;
  }
  score(Brickg);
  ck_assert_int_eq(Brickg->score, 1500);  ///< 4 линии = +1500 очков
  free_game(Brickg);
}
END_TEST

START_TEST(test_up_one) {  ///< Движение ВВЕРХ
  init_game();
  Brickg->pause = 0;
  userinput(Up, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Up);
  ck_assert_ptr_nonnull(Brickg->figure);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_up_two) {
  init_game();
  Brickg->pause = 0;
  for (int i = 0; i < Brickg->field->width; i++) {
    Brickg->field->blocks[2][i].b = 1;
  }
  userinput(Up, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Up);
  ck_assert_ptr_nonnull(Brickg->figure);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_plant_figure) {  ///< Строительство
  init_game();
  Brickg->pause = 0;
  plant_figure(Brickg);
  ck_assert_ptr_nonnull(Brickg->figure);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_one) {  ///< Ввод юзера
  init_game();
  Brickg->pause = 1;
  userinput(Left, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Left);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_two) {
  init_game();
  Brickg->pause = 0;
  userinput(Right, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Right);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_three) {
  init_game();
  Brickg->pause = 0;
  userinput(Down, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Down);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_four) {
  init_game();
  Brickg->pause = 0;
  userinput(Down, 0);
  for (int i = 0; i < Brickg->field->width; i++) {
    Brickg->field->blocks[1][i].b = 1;
  }
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Down);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_five) {
  init_game();
  userinput(Pause, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Pause);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_six) {
  init_game();
  userinput(Terminate, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Terminate);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_seven) {
  init_game();
  userinput(Start, 0);
  GameInfo_t game_info = updatecurrentstate();
  ck_assert_int_eq(Brickg->player->action, Start);
  free_gui(game_info, Brickg->form->size, Brickg->field->height);
  free_game(Brickg);
}
END_TEST

START_TEST(test_user_input_invalid) {
  init_game();
  userinput(999, 0);  ///< Некорректное действие
  ck_assert_int_eq(Brickg->player->action,
                   Action);  ///< Должно оставаться в Action
  free_game(Brickg);
}
END_TEST

START_TEST(test_calculate_one) {  ///< Счет, но не коммунальный
  init_game();
  Brickg->ticks = 1;
  Brickg->ticks_left = 0;
  Brickg->pause = 0;
  calculate_Brick(Brickg);
  ck_assert_int_eq(Brickg->ticks_left, Brickg->ticks);
  free_game(Brickg);
}
END_TEST

START_TEST(test_calculate_two) {
  init_game();
  Brickg->ticks_left = 0;
  Brickg->pause = 1;
  for (int i = 0; i < Brickg->field->width; i++) {
    Brickg->field->blocks[1][i].b = 1;
  }
  calculate_Brick(Brickg);
  ck_assert_int_eq(Brickg->ticks_left, Brickg->ticks);
  free_game(Brickg);
}
END_TEST

START_TEST(test_calculate_collision) {
  init_game();
  Brickg->ticks = 1;
  Brickg->ticks_left = 0;
  Brickg->pause = 0;
  while (!is_collision(Brickg)) {  ///< Перемещаем фигуру вниз
    move_figure_down(Brickg);
  }
  for (int i = 0; i < Brickg->field->width;
       i++) {  ///< Создаем ситуацию столкновения
    Brickg->field->blocks[Brickg->figure->y + 1][i].b = 1;
  }
  // printf("DEBUG: Перед вызовом calculate_Brick(), Brickg->state = %d\n",
  // Brickg->state);
  calculate_Brick(Brickg);
  // printf("DEBUG: После вызова calculate_Brick(), Brickg->state = %d\n",
  // Brickg->state);
  ck_assert_int_eq(Brickg->state, DROP);
  free_game(Brickg);
}
END_TEST

START_TEST(test_create_game) {  ///< Создание игры
  init_game();
  ck_assert_ptr_nonnull(Brickg);
  ck_assert_ptr_nonnull(Brickg->player);
  ck_assert_int_eq(Brickg->player->action, Start);
  ck_assert_ptr_nonnull(Brickg->figure);
  free_game(Brickg);
}
END_TEST

START_TEST(test_erase_lines) {  ///< красная линия?
  init_game();
  int line_erase = 18;
  for (int i = 0; i < Brickg->field->width; i++) {
    Brickg->field->blocks[line_erase][i].b = 1;
  }
  int erased = erase_lines(Brickg);
  ck_assert_int_eq(erased, 1);
  free_game(Brickg);
}
END_TEST

Suite* test() {
  Suite* suite = suite_create("TEST");

  TCase* tcase_score = tcase_create("SCORE");
  tcase_add_test(tcase_score, test_score);
  suite_add_tcase(suite, tcase_score);
  tcase_add_test(tcase_score, test_score);
  tcase_add_test(tcase_score, test_score_multiple_lines);
  tcase_add_test(tcase_score, test_score_four_lines);
  suite_add_tcase(suite, tcase_score);

  TCase* tcase_up = tcase_create("MOVE_FIGURE_UP");
  tcase_add_test(tcase_up, test_up_one);
  tcase_add_test(tcase_up, test_up_two);
  suite_add_tcase(suite, tcase_up);

  TCase* tcase_palnt = tcase_create("PLANT_FIGURE");
  tcase_add_test(tcase_palnt, test_plant_figure);
  suite_add_tcase(suite, tcase_palnt);

  TCase* tcase_input = tcase_create("USER_INPUT");
  tcase_add_test(tcase_input, test_user_input_one);
  tcase_add_test(tcase_input, test_user_input_two);
  tcase_add_test(tcase_input, test_user_input_three);
  tcase_add_test(tcase_input, test_user_input_four);
  tcase_add_test(tcase_input, test_user_input_five);
  tcase_add_test(tcase_input, test_user_input_six);
  tcase_add_test(tcase_input, test_user_input_seven);
  tcase_add_test(tcase_input, test_user_input_invalid);
  suite_add_tcase(suite, tcase_input);

  TCase* tcase_calculate_Brick = tcase_create("CALCULATE_Brick");
  tcase_add_test(tcase_calculate_Brick, test_calculate_one);
  tcase_add_test(tcase_calculate_Brick, test_calculate_two);
  tcase_add_test(tcase_calculate_Brick, test_calculate_collision);
  suite_add_tcase(suite, tcase_calculate_Brick);

  TCase* tcase_create_game = tcase_create("CREATE_GAME");
  tcase_add_test(tcase_create_game, test_create_game);
  suite_add_tcase(suite, tcase_create_game);

  TCase* tcase_erase_lines = tcase_create("ERASE_LINES");
  tcase_add_test(tcase_erase_lines, test_erase_lines);
  suite_add_tcase(suite, tcase_erase_lines);

  return suite;
}

int main() {
  int count;
  Suite* s = test();
  SRunner* runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  count = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}