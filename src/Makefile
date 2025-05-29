BRICKGAME_DIR := brick_game/tetris
TESTS_DIR := tests
GUI_DIR := gui/cli
BUILD_DIR := build
LCOVFLAGS = --ignore-errors inconsistent --ignore-errors mismatch

ifeq ($(shell uname -s), Linux)
	TEST_FLAGS = -lcheck -lpthread -pthread -lm  -lrt -lsubunit
else
	TEST_FLAGS = -lcheck -lpthread -pthread -lm  
endif

CC := gcc
TIMELIB := -D_POSIX_C_SOURCE=199309L # чтобы убрать ошибку с Posix
CFLAGS := -Wall -Werror -Wextra -std=c11
LDFLAGS := -lncurses
MKDIR_P := mkdir -p

SOURCES := $(BRICKGAME_DIR)/backend.c $(GUI_DIR)/frontend.c main.c
OBJECTS := $(BRICKGAME_DIR)/*.o $(GUI_DIR)/*.o

.PHONY: all clean install uninstall dvi dist test gcov_report

all: clean $(BUILD_DIR)/brickgame

install:
	@$(MKDIR_P) $(BUILD_DIR)
	$(CC) $(TIMELIB) $(CFLAGS) $(SOURCES) -o $(BUILD_DIR)/brickgame $(LDFLAGS)  

uninstall:
	rm -rf $(BUILD_DIR) $(OBJECTS)

$(BUILD_DIR)/brickgame:
	@$(MKDIR_P) $(@D)
	$(CC) $(TIMELIB) $(SOURCES) -o $@ $(LDFLAGS) $(CFLAGS)
	./$(BUILD_DIR)/brickgame

clean:
	rm -rf $(BUILD_DIR) $(OBJECTS) *.info gcov_report max_score.txt brickgame.tar.gz report doxygen_report

dvi: clean
	doxygen doc/Doxyfile
	open doxygen_report/html/index.html

dist: clean
	tar --exclude=brickgame.tar.gz --warning=no-file-changed -czvf brickgame.tar.gz . || true

test:
	@$(MKDIR_P) $(BUILD_DIR)
	$(CC) $(TIMELIB) $(CFLAGS) -g $(BRICKGAME_DIR)/backend.c $(TESTS_DIR)/*.c -o $(BUILD_DIR)/brickgame $(LDFLAGS)  $(TEST_FLAGS)
	./$(BUILD_DIR)/brickgame


gcov_report: clean
	@$(MKDIR_P) $(BUILD_DIR)  
	$(CC) $(TIMELIB) $(BRICKGAME_DIR)/backend.c $(TESTS_DIR)/*.c -o $(BUILD_DIR)/brickgame $(LDFLAGS) $(CFLAGS) $(TEST_FLAGS) -fprofile-arcs -ftest-coverage 
	./$(BUILD_DIR)/brickgame
	lcov -t "$(BRICKGAME_DIR)/brickgame.c" -o rep.info -c -d .
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcno *.gcda


style_fix:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -i */*/*.c */*/*.h */*.c *.c
	rm .clang-format

style:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -n */*/*.c */*/*.h */*.c *.c
	rm .clang-format

leaks: test
ifeq ($(shell uname -s), Linux)
	valgrind  --tool=memcheck --leak-check=yes  --show-leak-kinds=all ./$(BUILD_DIR)/brickgame   2>&1 | awk '/ERROR SUMMARY:/ { print }' 
else
	leaks --atExit -- 	./$(BUILD_DIR)/brickgame
endif
