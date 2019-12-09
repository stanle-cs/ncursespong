#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

typedef struct{
    char* model;
    char* modelDel;
    int x;
    int y;
} Object;


Object paddle1_top = {"|", " ", 0, 1};
Object paddle1_middle = {"|", " ", 0, 2};
Object paddle1_bottom = {"|", " ", 0, 3};

Object paddle2_top = {"|", " ", 0, 1};
Object paddle2_middle = {"|", " ", 0, 2};
Object paddle2_bottom = {"|", " ", 0, 3};

Object ball = {"o", " ", 0, 0};
int maxX = 0, maxY = 0;
int vX = 1, vY = 1;
int NEW_GAME_DELAY = 5;
int DELAY = 3;

static void finish(int sig);
void moveObject(int x, int y, Object object);
void eraseObject(int x, int y, Object object);
bool checkCollision(Object pad, Object bll);
void processKeystroke(int c) ;
static void ncursesInit();
void initScene();
void moveBall();
void movePaddle(int paddle, int direction);
static void finish(int sig)
{
    endwin();

    /* do your non-curses wrapup here */

    exit(0);
}

void moveObject(int x, int y, Object object) {
    char* model = object.model;
    mvprintw(y, x, "%s", model);
}

void eraseObject(int x, int y, Object object) {
    char* space = object.modelDel;
    mvprintw(object.y, object.x, "%s", space);

}

void processKeystroke(int c) {
    switch (c) {
        case KEY_UP:
            if (paddle1_top.y -1 >= 1) {
                moveObject(paddle1_top.x, paddle1_top.y - 1, paddle1_top);
                moveObject(paddle1_middle.x, paddle1_middle.y - 1, paddle1_middle);
                moveObject(paddle1_bottom.x, paddle1_bottom.y - 1, paddle1_bottom);
                eraseObject(paddle1_bottom.x, paddle1_bottom.y, paddle1_bottom);
                paddle1_top.y -= 1;
                paddle1_middle.y -= 1;
                paddle1_bottom.y -= 1;
            }
            break;
        case KEY_DOWN:
            if (paddle1_bottom.y + 1 <= maxY) {
                moveObject(paddle1_top.x, paddle1_top.y + 1, paddle1_top);
                moveObject(paddle1_middle.x, paddle1_middle.y + 1, paddle1_middle);
                moveObject(paddle1_bottom.x, paddle1_bottom.y + 1, paddle1_bottom);
                eraseObject(paddle1_top.x, paddle1_top.y, paddle1_top);
                paddle1_top.y += 1;
                paddle1_middle.y += 1;
                paddle1_bottom.y += 1;
            }
            break;
        case 's':
            moveBall();
            break;
    }
}


static void ncursesInit() {
    (void) initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) nodelay(stdscr, 1);  /* tell ncurses to trhow ERR if getch() waits too long
                                 * this means that getch() won't pause program execution */
    (void) cbreak();       /* take input chars one at a time, no wait for \n */
    (void) noecho();         /* echo input - in color */
    (void) curs_set(0);    
    box(stdscr, 0, 0);
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

bool checkCollision(Object pad, Object bll) {
    if ((pad.x == bll.x) && (pad.y == bll.y)) return true;
    else return false;
}

/**
 *  Move the ball and check for collision
 */
void moveBall() {
    eraseObject(ball.x, ball.y, ball);
    ball.x += vX;
    if ((ball.y + vY >= maxY - 1) || (ball.y + vY <= 0)) vY = -vY;
    ball.y += vY;
    if (ball.x > maxX) ball.x = maxX;
    else if (ball.x < 0) ball.x = 0;
    else if (ball.y > maxY) ball.y = maxY;
    else if (ball.y < 0) ball.y = 0;
    moveObject(ball.x, ball.y, ball);
    if (ball.x == 0) {
        mvprintw(maxY / 2, maxX / 2 - 4, "YOU LOSE!");
        getch();
        usleep(NEW_GAME_DELAY*100000);
        initScene();
    }
    if (ball.x == maxX) {
        mvprintw(maxY / 2, maxX / 2 - 4, "YOU WIN!");
        getch();
        usleep(NEW_GAME_DELAY*100000);
        initScene();
    }
}

void movePaddle(int pad, int direction) {
    Object paddle_top, paddle_middle, paddle_bottom;
    if (pad == 1) {
        paddle_top = paddle1_top;
        paddle_middle = paddle1_middle;
        paddle_bottom = paddle1_bottom;
    }
    else if (pad == 2) {
        paddle_top = paddle2_top;
        paddle_middle = paddle2_middle;
        paddle_bottom = paddle2_bottom;
    }
    
    switch (direction) {
        case KEY_UP:
            if (paddle_top.y >= 1) {
                moveObject(paddle_top.x, paddle_top.y - 1, paddle_top);
                moveObject(paddle_middle.x, paddle_middle.y - 1, paddle_middle);
                moveObject(paddle_bottom.x, paddle_bottom.y - 1, paddle_bottom);
                eraseObject(paddle_bottom.x, paddle_bottom.y, paddle_bottom);
                paddle_top.y -= 1;
                paddle_middle.y -= 1;
                paddle_bottom.y -= 1;
            }
            break;
        case KEY_DOWN:
            if (paddle_bottom.y + 2 <= maxY) {
                moveObject(paddle_top.x, paddle_top.y + 1, paddle_top);
                moveObject(paddle_middle.x, paddle_middle.y + 1, paddle_middle);
                moveObject(paddle_bottom.x, paddle_bottom.y + 1, paddle_bottom);
                eraseObject(paddle_top.x, paddle_top.y, paddle_top);
                paddle_top.y += 1;
                paddle_middle.y += 1;
                paddle_bottom.y += 1;
            }
            break;
    }
}

void resolveCollision(int paddle) {
    if (paddle == 1) {
        if (checkCollision(paddle1_top, ball)) vX = -vX;
        else if (checkCollision(paddle1_middle, ball)) vX = -vX;
        else if (checkCollision(paddle1_bottom, ball)) vX = -vX;
    } else if (paddle == 2) {
        if (checkCollision(paddle2_top, ball)) vX = -vX;
        else if (checkCollision(paddle2_middle, ball)) vX = -vX;
        else if (checkCollision(paddle2_bottom, ball)) vX = -vX;
    }
}

void initScene() {
    vX = 1, vY = 1;
    clear();
    getmaxyx(stdscr, maxY, maxX);
    int offset =6;

    paddle1_top.x = offset - 1;
    paddle1_middle.x = offset - 1;
    paddle1_bottom.x = offset - 1;

    moveObject(paddle1_top.x, paddle1_top.y, paddle1_top);
    moveObject(paddle1_middle.x, paddle1_middle.y, paddle1_middle);
    moveObject(paddle1_bottom.x, paddle1_bottom.y, paddle1_bottom);

    paddle2_top.x = maxX - offset;
    paddle2_middle.x = maxX - offset;
    paddle2_bottom.x = maxX - offset;

    moveObject(paddle2_top.x, paddle2_top.y, paddle2_top);
    moveObject(paddle2_middle.x, paddle2_middle.y, paddle2_middle);
    moveObject(paddle2_bottom.x, paddle2_bottom.y, paddle2_bottom);
    

    moveObject(maxX / 2, maxY / 2, ball);
    ball.x = maxX / 2;
    ball.y = maxY / 2;
}

void autoPlay() {
    
    eraseObject(paddle2_top.x, paddle2_top.y, paddle2_top);
    eraseObject(paddle2_middle.x, paddle2_middle.y, paddle2_middle);
    eraseObject(paddle2_bottom.x, paddle2_bottom.y, paddle2_bottom);

    paddle2_middle.y = ball.y + (rand() % (2 + 2 + 1) - 2);
    paddle2_bottom.y = paddle2_middle.y + 1;
    paddle2_top.y = paddle2_middle.y - 1;

    moveObject(paddle2_top.x, paddle2_top.y, paddle2_top);
    moveObject(paddle2_middle.x, paddle2_middle.y, paddle2_middle);
    moveObject(paddle2_bottom.x, paddle2_bottom.y, paddle2_bottom);

}
int main(int argc, char *argv[]) {
    srand(time(0));
    int num = 0;
    int currentX = 0, currentY = 0;
    ncursesInit();
    initScene();

    /* initialize your non-curses data structures here */
    for (;;) {
        attrset(COLOR_PAIR(rand() % 8));
        num++;
        mvprintw(maxY - 1, maxX - 10, "%d", num);
        int c = getch();     /* refresh, accept single keystroke of input */
        /* process the command keystroke */
        if (c != ERR) {
            processKeystroke(c);
            if (c == 'Q') break;
        }
        usleep(DELAY*10000);
        moveBall();
        autoPlay();
        resolveCollision(1);
        resolveCollision(2);
    }

    finish(0);               /* we are done */
}

