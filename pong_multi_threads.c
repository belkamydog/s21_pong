#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

const int ROW = 25;
const int COL = 80;
const int GAME_OVER_SCORE = 21;
const int GAME_SPEED = 300;

const char BALL = '@';
const char ROCKET = '|';
const char HORISONTAL_BORDER = '-';
const char VERTICAL_BORDER = '+';

typedef struct ball {
  int x, y, vector_x, vector_y;
} Ball;

typedef struct player {
  int position;
  int score;
} Player;

typedef struct game {
  Ball ball;
  Player player_1;
  Player player_2;
  char game_over;
  char user_input;
  int ball_speed;
} Game;

void main_render(Game game);
void score_render(int p1, int p2);
char rockets_render(int x, int y, int r_y_p1, int r_y_p2, char is_printed);
void field_render(int y, int x, char is_printed);
char ball_render(int x, int y, Ball ball);

void init_game(Game *game);
void moove_ball(Game *game);
void moove_rockets(Game *game);
void is_game_over(char input, Game *game);

// gameplay
void init_game(Game *game) {
  game->ball.x = COL / 2;
  game->ball.y = ROW / 2;
  game->ball.vector_x = 1;
  game->ball.vector_y = 1;
  game->player_1.score = 0;
  game->player_2.score = 0;
  game->player_1.position = ROW / 2;
  game->player_2.position = ROW / 2;
  game->game_over = 1;
  game->user_input = ' ';
  game->ball_speed = GAME_SPEED * 1000;
}

void is_game_over(char input, Game *game) {
  char result = 1;
  if (input == 'q' || input == 'Q')
    result = 0;
  else if (game->player_1.score == GAME_OVER_SCORE ||
           game->player_2.score == GAME_OVER_SCORE)
    result = 0;
  game->game_over = result;
}

void moove_ball(Game *game) {
  if (game->ball.y == 1 || game->ball.y == (ROW - 2)) game->ball.vector_y *= -1;
  game->ball.y += game->ball.vector_y;
  if (game->ball.x == 2 && (game->ball.y == game->player_1.position ||
                            game->ball.y == game->player_1.position - 1 ||
                            game->ball.y == game->player_1.position + 1))
    game->ball.vector_x *= -1;
  if (game->ball.x == (COL - 3) &&
      (game->ball.y == game->player_2.position ||
       game->ball.y == game->player_2.position - 1 ||
       game->ball.y == game->player_2.position + 1))
    game->ball.vector_x *= -1;
  game->ball.x += game->ball.vector_x;
  if (game->ball.x == COL - 1) game->player_1.score++;
  if (game->ball.x == 0) game->player_2.score++;
  if (game->ball.x == 0 || game->ball.x == (COL - 1)) {
    game->ball.vector_x *= -1;
    game->ball.x = COL / 2;
    game->ball.y = ROW / 2;
  }
}

// rendering

void main_render(Game game) {
  for (int y = 0; y < ROW; y++) {
    for (int x = 0; x < COL; x++) {
      char printed = 0;
      printed = ball_render(x, y, game.ball);
      printed = rockets_render(x, y, game.player_1.position,
                               game.player_2.position, printed);
      field_render(y, x, printed);
    }
    printw("\n");
  }
  score_render(game.player_1.score, game.player_2.score);
}

char ball_render(int x, int y, Ball ball) {
  char is_printed = 0;
  if (y == ball.y && x == ball.x) {
    printw("%c", BALL);
    is_printed = 1;
  }
  return is_printed;
}

void field_render(int y, int x, char is_printed) {
  if (is_printed == 0) {
    if (y == 0 || y == (ROW - 1))
      printw("%c", HORISONTAL_BORDER);
    else if (x == 0 || x == (COL - 1))
      printw("%c", VERTICAL_BORDER);
    else
      printw(" ");
  }
}

char rockets_render(int x, int y, int r_y_p1, int r_y_p2, char is_printed) {
  if (is_printed == 0) {
    if (x == 1) {
      if (y == r_y_p1) {
        printw("%c", ROCKET);
        is_printed = 1;
      } else if (y == r_y_p1 - 1) {
        printw("%c", ROCKET);
        is_printed = 1;
      } else if (y == r_y_p1 + 1) {
        printw("%c", ROCKET);
        is_printed = 1;
      }
    } else if (x == (COL - 2)) {
      if (y == r_y_p2) {
        printw("%c", ROCKET);
        is_printed = 1;
      } else if (y == r_y_p2 - 1) {
        printw("%c", ROCKET);
        is_printed = 1;
      } else if (y == r_y_p2 + 1) {
        printw("%c", ROCKET);
        is_printed = 1;
      }
    }
  }
  return is_printed;
}

void score_render(int p1, int p2) {
  printw("score:\n");
  printw("p1: %2d p2: %2d\n", p1, p2);
}

void moove_rockets(Game *game) {
  char ch = game->user_input;
  if (game->ball.vector_x == -1) {
    if ((ch == 'z' || ch == 'Z') && game->player_1.position < ROW - 3)
      game->player_1.position += 1;
    else if ((ch == 'a' || ch == 'A') && game->player_1.position > 2)
      game->player_1.position -= 1;
  } else if (game->ball.vector_x == 1) {
    if ((ch == 'm' || ch == 'M') && game->player_2.position < ROW - 3)
      game->player_2.position += 1;
    else if ((ch == 'k' || ch == 'K') && game->player_2.position > 2)
      game->player_2.position -= 1;
  }
}

void *ball_thread_func(void *arg) {
  Game *game = (Game *)arg;
  while (game->game_over == 1) {
    moove_ball((Game *)arg);
    usleep(game->ball_speed);
  }
  return NULL;
}

int main() {
  Game game;
  pthread_t ball_thread;
  init_game(&game);
  pthread_create(&ball_thread, NULL, ball_thread_func, &game);
  initscr();
  keypad(stdscr, true);
  raw();
  char user_input = ' ';
  while (game.game_over != 0) {
    is_game_over(user_input, &game);
    timeout(GAME_SPEED);
    main_render(game);
    user_input = getch();
    game.user_input = user_input;
    moove_rockets(&game);
    erase();
  }
  pthread_exit(NULL);
  endwin();
  return 0;
}
