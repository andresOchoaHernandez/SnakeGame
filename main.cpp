/*
    AUTHOR: Andres Ochoa Hernandez, computer science student of University of Verona.

    This is my very first game I ever programmed. I made it with the following purposes:
        
        -gain basic game programming experience
        -practise C++ programming language
        -learn something about ncurses library
        -challenge myself and have fun!

    SNAKE GAME PROGRAM:
        -GRID:
            Is a GAME_WIN_HEIGTH x GAME_WIN_WIDTH window.
        -SNAKE:
            Snake is a struct which holds:
                -length  -> its length                                  
                -snakeBody -> vector of bodyPart(s)
                -head is the snakeBody[snake.legth-1] element of the vector
            BodyPart is a struct which holds:                      0 1
                -Pos -> current coordinates of bodyPart in format [y x]
                -direction -> 0 up, 1 rigth, 2 down, 3 left
            Snake Default length is 3. It can be modified by rewriting InitSnake() fuction.
                                             
*/

#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <sys/signal.h>

#define BORDER_WIN_X 1
#define BORDER_WIN_Y 0
#define BORDER_WIN_WIDTH 78
#define BORDER_WIN_HEIGTH 21

#define GAME_WIN_X 2
#define GAME_WIN_Y 1
#define GAME_WIN_WIDTH 76
#define GAME_WIN_HEIGTH 19

#define TITLE_WIN_HEIGTH 3
#define TITLE_WIN_WITDH 20
#define TITLE_WIN_Y 21
#define TITLE_WIN_X 1

#define SCORE_WIN_HEIGTH 3
#define SCORE_WIN_WITDH 12
#define SCORE_WIN_Y 21
#define SCORE_WIN_X 67

using namespace std;

struct BODYPART{   
    int pos[2];
    int direction;
}
typedef BODYPART;

struct SNAKE{
    int length;
    vector<BODYPART> snakeBody;
}
typedef SNAKE;

struct FOOD{
    int pos[2];
}
typedef FOOD;

SNAKE initSnake(void);
FOOD initFood(void);
void drawSnake(WINDOW *gameWin,SNAKE *snake);
void updateSnake(SNAKE *snake,int direction);
void thick(WINDOW *gameWin,SNAKE *snake,int direction);
void drawFood(WINDOW *gameWin,FOOD *food);
void growSnake(SNAKE *snake);
void updateScore(WINDOW *scoreWin,int *score);
bool checkIfTouches(SNAKE *snake);

int main(int argc,char *argv[]){

    initscr();
    cbreak();
    curs_set(0);

    /*MAIN SCREEN SETTINGS*/
    WINDOW *inputWin=newwin(1,1,TITLE_WIN_Y+1,TITLE_WIN_X + TITLE_WIN_WITDH +13);
    WINDOW *borderWin=newwin(BORDER_WIN_HEIGTH,BORDER_WIN_WIDTH,BORDER_WIN_Y,BORDER_WIN_X);
    WINDOW *gameWin=newwin(GAME_WIN_HEIGTH,GAME_WIN_WIDTH,GAME_WIN_Y,GAME_WIN_X);
    WINDOW *titleWin=newwin(TITLE_WIN_HEIGTH,TITLE_WIN_WITDH,TITLE_WIN_Y,TITLE_WIN_X);
    WINDOW *scoreWin=newwin(SCORE_WIN_HEIGTH,SCORE_WIN_WITDH,SCORE_WIN_Y,SCORE_WIN_X);

    box(borderWin,0,0);
    box(titleWin,0,0);
    box(scoreWin,0,0);

    mvwprintw(scoreWin,1,1,"SCORE: 0");
    mvwprintw(titleWin,1,1,"S N A K E  G A M E");
    mvprintw(TITLE_WIN_Y+1,TITLE_WIN_X + TITLE_WIN_WITDH +14,"PRESS 'E' TO EXIT");

    refresh();

    wrefresh(borderWin);
    wrefresh(gameWin);
    wrefresh(titleWin);
    wrefresh(scoreWin);

    /*GAME LOGIC*/
    nodelay(inputWin,true);

    /*Init snake*/
    begin:

    wclear(gameWin);
    SNAKE snake=initSnake();
    FOOD food=initFood();

    drawSnake(gameWin,&snake);
    drawFood(gameWin,&food);
    
    /*GAME LOOP*/
    int score=0;
  
    while(1){
        switch (wgetch(inputWin))
        {
        case 65:
            wdelch(inputWin);

            if(snake.snakeBody[snake.length-1].direction==2){
                thick(gameWin,&snake,2);
                break;
            }

            thick(gameWin,&snake,0);
            break;
        case 66:
            wdelch(inputWin);

            if(snake.snakeBody[snake.length-1].direction==0){
                thick(gameWin,&snake,0);
                break;
            }

            thick(gameWin,&snake,2);
            break;
        case 67:
            wdelch(inputWin);

            if(snake.snakeBody[snake.length-1].direction==3){
                thick(gameWin,&snake,3);
                break;
            }

            thick(gameWin,&snake,1);
            break;
        case 68:
            wdelch(inputWin);   

            if(snake.snakeBody[snake.length-1].direction==1){
                thick(gameWin,&snake,1);
                break;
            }

            thick(gameWin,&snake,3);
            break;
        case 'e':
            endwin();
            kill(getpid(),SIGINT);
            break;     
        default:
            wdelch(inputWin);
            thick(gameWin,&snake,snake.snakeBody[snake.length-1].direction);
            break;
        }

        if(snake.snakeBody[snake.length-1].pos[0]==food.pos[0] && snake.snakeBody[snake.length-1].pos[1]==food.pos[1]){
            updateScore(scoreWin,&score);
            food=initFood();
            drawFood(gameWin,&food);
            growSnake(&snake);
        }
        if(checkIfTouches(&snake)){
            wclear(gameWin);
            mvwprintw(gameWin,GAME_WIN_HEIGTH/2+2,GAME_WIN_WIDTH/2-5,"GAME OVER");
            mvwprintw(gameWin,GAME_WIN_HEIGTH/2,GAME_WIN_WIDTH/2-9,"PRESS 'E' TO EXIT");
            mvwprintw(gameWin,GAME_WIN_HEIGTH/2-2,GAME_WIN_WIDTH/2-9,"PRESS 'R' TO RETRY");
            wrefresh(gameWin);

            while(1){

                switch (wgetch(inputWin))
                {
                case 'e':
                    wdelch(inputWin);
                    endwin();
                    kill(getpid(),SIGINT);
                    break;
                case 'r':
                    wdelch(inputWin);
                    for(int i=1;i<=10;i++)
                        mvwaddch(scoreWin,1,i,' ');
                    mvwprintw(scoreWin,1,1,"SCORE: 0");
                    wrefresh(scoreWin);
                    goto begin;
                    break;
                default:
                    wdelch(inputWin);
                    break;
                }

            }
           
        }
    }

    /*END OF THE PROGRAM*/
    return 0;
}

bool checkIfTouches(SNAKE *snake){
    
    vector<BODYPART>tmp;
    for(auto elem:snake->snakeBody){
        tmp.push_back(elem);
    }

    for(int i=0;i<snake->length-2;i++){
        if(snake->snakeBody[snake->length-1].pos[0]==tmp[i].pos[0] && snake->snakeBody[snake->length-1].pos[1]==tmp[i].pos[1] )
            return true;
    }

    return false;
}

void updateScore(WINDOW *scoreWin,int *score){
    *score=*score+10;
    mvwprintw(scoreWin,1,8,"%i",*score);
    wrefresh(scoreWin);
}

void growSnake(SNAKE *snake){
    BODYPART newBodyPart;
    newBodyPart.direction=snake->snakeBody[0].direction;
    newBodyPart.pos[0]=snake->snakeBody[0].pos[0];
    newBodyPart.pos[1]=snake->snakeBody[0].pos[1];
    snake->snakeBody.insert(snake->snakeBody.begin(),newBodyPart);
    snake->length++;
}

void drawFood(WINDOW *gameWin,FOOD *food){
    mvwprintw(gameWin,food->pos[0],food->pos[1],"+");
    wrefresh(gameWin);
}

void thick(WINDOW *gameWin,SNAKE *snake,int direction){

    mvwaddch(gameWin,snake->snakeBody[0].pos[0],snake->snakeBody[0].pos[1],' ');
    updateSnake(snake,direction);
    drawSnake(gameWin,snake);
    wrefresh(gameWin);
    usleep(90000);
}

FOOD initFood(void){
    
    srand(time(nullptr));

    FOOD food;
    food.pos[0]=rand()%(GAME_WIN_HEIGTH-1);
    food.pos[1]=rand()%(GAME_WIN_WIDTH-1);

    return food;
}

SNAKE initSnake(void){
    srand(time(nullptr));

    SNAKE snake;
    snake.length=3;

    BODYPART head;
    head.direction=1;
    head.pos[0]=rand()%(GAME_WIN_HEIGTH);
    head.pos[1]=rand()%(GAME_WIN_WIDTH-snake.length)+snake.length;

    BODYPART body;
    body.direction=head.direction;
    body.pos[0]=head.pos[0];
    body.pos[1]=head.pos[1]-1;

    BODYPART tail;
    tail.direction=body.direction;
    tail.pos[0]=body.pos[0];
    tail.pos[1]=body.pos[1]-1;

    snake.snakeBody={tail,body,head};

    return snake;
}

void updateSnake(SNAKE *snake,int direction){

    vector<BODYPART> tmp;
    for(auto elem:snake->snakeBody){
          tmp.push_back(elem);
    }

    snake->snakeBody[snake->length-1].direction=direction;
    
    switch (direction)
    {
    case 0:
        snake->snakeBody[snake->length-1].pos[0]=(snake->snakeBody[snake->length-1].pos[0]-1)==-1?GAME_WIN_HEIGTH-1:(snake->snakeBody[snake->length-1].pos[0]-1);
        break;
    
    case 1:
        snake->snakeBody[snake->length-1].pos[1]=(snake->snakeBody[snake->length-1].pos[1]+1)%(GAME_WIN_WIDTH);
        break;
    
    case 2:
        snake->snakeBody[snake->length-1].pos[0]=(snake->snakeBody[snake->length-1].pos[0]+1)%(GAME_WIN_HEIGTH);
        break;

    case 3:
        snake->snakeBody[snake->length-1].pos[1]=(snake->snakeBody[snake->length-1].pos[1]-1)==-1?GAME_WIN_WIDTH-1:snake->snakeBody[snake->length-1].pos[1]-1;
        break;
    
    default:
        break;
    }

    for(int i=0;i<snake->length-1;i++){
        if(i+1==snake->length)
            break;
        else{
            snake->snakeBody[i].direction=tmp[i+1].direction;
            snake->snakeBody[i].pos[0]=tmp[i+1].pos[0];
            snake->snakeBody[i].pos[1]=tmp[i+1].pos[1];
        }
    }
}

void drawSnake(WINDOW *gameWin,SNAKE *snake){

    for(auto x: snake->snakeBody){
       mvwprintw(gameWin,x.pos[0],x.pos[1],"#");
    }

    wrefresh(gameWin);
}
