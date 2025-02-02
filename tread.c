#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

void* ball(void* arg){
    while(1){
        *(int *) arg += 1;
        sleep(1);
    }
}

int main(){
    pthread_t t1;
    int i = 0;
    pthread_create(&t1, NULL, ball, &i);
    initscr();
    while (1){
        printw("%d", i);
        refresh();
        erase();
    }
    pthread_exit(NULL);
    endwin();
    return 0;
}