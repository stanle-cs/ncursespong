#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

typedef struct{
    char* model;
    char* modelDel;
    int x;
    int y;
} Object;


Object paddle = {"|", " ", 0, 0};
Object ball = {"o", " ", 0, 0};

static void finish(int sig)
{
    endwin();

    /* do your non-curses wrapup here */

    exit(0);
}

void processKeystroke(int c, int* x, int* y, Object *paddle) {
    char* player = paddle->model;
    char* space = paddle->modelDel;
    switch (c) {
        case KEY_UP:
            (void) mvwprintw(stdscr, *y, *x, "%s", space);
            (void) mvwprintw(stdscr, *y - 1, *x, "%s", player);
            *y -= 1;
            break;
        case KEY_DOWN:
            (void) mvwprintw(stdscr, *y, *x, "%s", space);
            (void) mvwprintw(stdscr, *y + 1, *x, "%s", player);
            *y += 1;
            break;
        /*
        case KEY_LEFT:
            (void) mvwprintw(stdscr, *y, *x, "%s", space);
            (void) mvwprintw(stdscr, *y, *x - 1, "%s", player);
            *x -= 1;
            break;
        case KEY_RIGHT:
            (void) mvwprintw(stdscr, *y, *x, "%s", space);
            (void) mvwprintw(stdscr, *y, *x + 1, "%s", player);
            *x += 1;
            break;
        */    
    }
}
int maxX, maxY;

static void ncursesInit() {
    /* arrange interrupts to terminate */
    (void) initscr();      /* initialize the curses library */
    box(stdscr, 0, 0);
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) nodelay(stdscr, 1);  /* tell ncurses to trhow ERR if getch() waits too long
                                 * this means that getch() won't pause program execution */
    (void) cbreak();       /* take input chars one at a time, no wait for \n */
    (void) echo();         /* echo input - in color */
    (void) curs_set(0);    
    if (has_colors()) {
        start_color();

        /*
         * Simple color assignment, often all we need.  Color pair 0 cannot
         * be redefined.  This example uses the same value for the color
         * pair as for the foreground color, though of course that is not
         * necessary:
         */
        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
    }
}

bool checkCollision(Object paddle, Object ball) {
    if ((paddle.x == ball.x) && (paddle.y == ball.y)) {
        return true;
    }
    else return false;
}

int vX = 1, vY = 1;
void moveBall(int maxX, int maxY) {
    mvprintw(ball.x, ball.y, ball.modelDel);
    if ((ball.x + vX > maxX) || (ball.x + vX < 0)) {
        vX = -vX;
    }
    ball.x += vX;
    if ((ball.y + vY > maxY) || (ball.y + vY < 0)) {
        vY = -vY;
    }
    ball.y += vY;

    if (ball.x > maxX) ball.x = maxX;
    if (ball.x < 0) ball.x = 0;
    if (ball.y > maxY) ball.y = maxY;
    if (ball.y < 0) ball.y = 0;

    mvprintw(ball.x, ball.y, ball.model);
}

int main(int argc, char *argv[]) {
    
    int num = 0;
    int currentX = 0, currentY = 0;
    usleep(3000000);
    ncursesInit();
    
    /* initialize your non-curses data structures here */
    for (;;)
    {
        usleep(100000);
        getmaxyx(stdscr, maxY, maxX);
        int c = getch();     /* refresh, accept single keystroke of input */
        moveBall(maxX, maxY);
        attrset(COLOR_PAIR(num % 8));
        num++;
        mvprintw(20, 20, "%d", num);
        mvprintw(21, 20, "                                            ");
        mvprintw(21, 20, "%d %d - %d %d ", paddle.x, paddle.y, currentX, currentY);
        /* process the command keystroke */
        if (c != ERR) {
            processKeystroke(c, &currentX, &currentY, &paddle);
 
            paddle.x = currentX;
            paddle.y = currentY;
            if (checkCollision(paddle, ball)) mvwprintw (stdscr, 23, 20, "hit!");
            mvprintw(22, 20, "                                            ");
            mvwprintw (stdscr, 22, 20, "paddle: %d %d - ball: %d %d", paddle.x, paddle.y, ball.x, ball.y);
            if (c == 'Q') break;
        }
    }

    finish(0);               /* we are done */
}

