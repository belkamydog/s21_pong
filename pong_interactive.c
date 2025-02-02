#include <pthread.h>
#include <stdio.h>
#include <ncurses.h>

const int ROW = 25;
const int COL = 80;
const int GAME_OVER_SCORE = 21;
const int GAME_SPEED = 500;

const char BALL =  '@';
const char ROCKET = '|';
const char HORISONTAL_BORDER = '-';
const char VERTICAL_BORDER = '+';


void main_render(int ball_x, int ball_y, int rocket_p1, int rocket_p2, int score_p1, int score_p2);
char rockets_render(int x, int y, int r_y_p1, int r_y_p2, char is_printed);
char ball_render(int x, int y, int b_x, int b_y);
void field_render(int y, int x, char is_printed);
void score_render(int p1, int p2);
void show_final_score(int score_p1, int score_p2);

int move_r1(char ch, int r1_y, int vector_ball_x);
int move_r2(char ch, int r2_y, int vector_ball_x);

int change_vector_y(int coord, int vector);
int move_ball_y(int coord, int vector);
int move_ball_x(int coord, int vector);
int hit_p1(int x_ball, int y_ball, int vector, int r_coord);
int hit_p2(int x_ball, int y_ball, int vector, int r_coord);
int check_score_p1(int score_p1, int x_ball);
int check_score_p2(int score_p2, int x_ball);
int reset_ball_coord_x(int coord);
int reset_ball_coord_y(int y_ball, int x_ball);
int reset_ball_vector_x(int vector, int coord);
char is_game_over(char input, int score_p1, int score_p2);


int main(){
    int score_p1 = 0, score_p2 = 0;
    int ball_x = COL/2, ball_y = ROW/2;
    int vector_ball_x = 1, vector_ball_y = -1;
    int r1_y = ROW/2, r2_y = ROW/2;
    char user_input = ' ';
    initscr();
    keypad(stdscr,true);
    raw();
    while (is_game_over(user_input, score_p1, score_p2) == 1) {
            timeout(GAME_SPEED);
            main_render(ball_x, ball_y, r1_y, r2_y, score_p1, score_p2);
            user_input = getch();
            r1_y = move_r1(user_input, r1_y, vector_ball_x);
            r2_y = move_r2(user_input, r2_y, vector_ball_x);
        if(user_input == -1){
            vector_ball_y = change_vector_y(ball_y, vector_ball_y);
            ball_y = move_ball_y(ball_y, vector_ball_y);
            vector_ball_x = hit_p1(ball_x, ball_y, vector_ball_x, r1_y);
            vector_ball_x = hit_p2(ball_x, ball_y, vector_ball_x, r2_y);
            ball_x = move_ball_x(ball_x, vector_ball_x);
            score_p1 = check_score_p1(score_p1, ball_x);
            score_p2 = check_score_p2(score_p2, ball_x);
            vector_ball_x = reset_ball_vector_x(vector_ball_x, ball_x);
            ball_y = reset_ball_coord_y(ball_y, ball_x);
            ball_x = reset_ball_coord_x(ball_x);
        }
        erase();
    }
    show_final_score(score_p1, score_p2);
    endwin();
    return 0;
}


// gameplay
int move_r1(char ch, int r1_y, int vector_ball_x){
    if (vector_ball_x == -1){
        if ((ch == 'z' || ch == 'Z') && r1_y < ROW-3) r1_y++;
        else if ((ch == 'a' || ch == 'A') && r1_y > 2) r1_y--;
    }
    return r1_y;
}

int move_r2(char ch, int r2_y, int vector_ball_x){
    if (vector_ball_x == 1){
        if ((ch == 'm' || ch == 'M') && r2_y < ROW-3) r2_y++;
        else if ((ch == 'k' || ch == 'K') && r2_y > 2) r2_y--;
    }
    return r2_y;
}

int move_ball_x(int coord, int vector) {
    coord += vector;
    return coord;
}

int move_ball_y(int coord, int vector) {
    coord += vector;
    return coord;
}

int change_vector_y(int coord, int vector){
    if (coord == 1 || coord == (ROW-2)) vector *= -1;
    return vector;
}

int hit_p1(int x_ball, int y_ball, int vector, int r_coord){
    if (x_ball == 2 && (y_ball == r_coord || y_ball == r_coord-1 || y_ball == r_coord+1 )) vector *= -1;
    return vector;
}

int hit_p2(int x_ball, int y_ball, int vector, int r_coord){
    if (x_ball == COL-3 && (y_ball == r_coord || y_ball == r_coord-1 || y_ball == r_coord+1)) vector *= -1;
    return vector;
}

int check_score_p1(int score_p1, int x_ball){
    if (x_ball == COL-1) score_p1++;
    return score_p1;
}

int check_score_p2(int score_p2, int x_ball){
    if (x_ball == 0) score_p2++;
    return score_p2;
}

int reset_ball_coord_x(int coord){
    if (coord == 0 || coord == (COL-1)) coord = COL/2;
    return coord;
}

int reset_ball_vector_x(int vector, int coord){
    if (coord == 0 || coord == (COL-1)) vector *= -1;
    return vector;
}

int reset_ball_coord_y(int y_ball, int x_ball){
    if (x_ball == 0 || x_ball == (COL-1)) y_ball = ROW/2;
    return y_ball;
}

char is_game_over(char input, int score_p1, int score_p2){
    char result = 1;
    if (input == 'q' || input == 'Q') result = 0;
    else if (score_p1 == GAME_OVER_SCORE || score_p2 == GAME_OVER_SCORE) result = 0;
    return result;
}

// rendering

void main_render(int ball_x, int ball_y, int rocket_p1, int rocket_p2, int score_p1, int score_p2){
    for (int y = 0; y < ROW; y++){
        for (int x = 0; x < COL; x++){
            char printed = 0;
            printed = ball_render(x, y, ball_x, ball_y);
            printed = rockets_render(x, y, rocket_p1, rocket_p2, printed);
            field_render(y, x, printed);
        }
        printw("\n");
    }
    score_render(score_p1, score_p2);
}

char ball_render(int x, int y, int b_x, int b_y){
    char is_printed = 0;
    if (y == b_y && x == b_x){
        printw("%c", BALL);
        is_printed = 1;
    }
    return is_printed;
}

void field_render(int y, int x, char is_printed){
    if (is_printed == 0){
        if (y == 0 || y == (ROW-1)) printw("%c", HORISONTAL_BORDER);
        else if (x == 0 || x == (COL-1)) printw("%c", VERTICAL_BORDER);
        else printw(" ");
    }
}

char rockets_render(int x, int y, int r_y_p1, int r_y_p2, char is_printed){
    if (is_printed == 0){
        if (x == 1){
            if (y == r_y_p1){
                printw("%c", ROCKET);
                is_printed = 1;
            } 
            else if (y == r_y_p1-1){
                printw("%c", ROCKET);
                is_printed = 1;
            }
            else if (y == r_y_p1+1) {
                printw("%c", ROCKET);
                is_printed = 1;
            }
        }
        else if (x == (COL-2)){
            if (y == r_y_p2){
                printw("%c", ROCKET);
                is_printed = 1;
            } 
            else if (y == r_y_p2-1){
                printw("%c", ROCKET);
                is_printed = 1;
            }
            else if (y == r_y_p2+1) {
                printw("%c", ROCKET);
                is_printed = 1;
            }
        }
    }
    return is_printed;
}

void score_render(int p1, int p2){
    printw("score:\n");
    printw("p1: %2d p2: %2d\n", p1, p2);
}

void show_final_score(int score_p1, int score_p2){
    if (score_p1 > score_p2) printw("Player 1 win\n");
    else if (score_p1 == score_p2) printw("Draw\n");
    else printw("Player 2 win\n");
}
