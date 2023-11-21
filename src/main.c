#include "../lib/Canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Canvas* canvas;
int width = 30;
int height = 30;
int score = 0;

typedef struct part{
  int x,y;
  struct part *next;
}Part;

char headChar[] = BG;
char tailChar[] = BG;
char appleChar[] = "🍎";

Part* newPart(int x, int y);
void drawSnake(Canvas* canvas, Part* head);
void addSnake(Part* head,int nx, int ny);
void popPart(Part* head);
void moveSnake(Part* head, int nx, int ny,int *add);
void freeSnake(Part* head);

void freeSnake(Part* head){
  while(head->next != NULL){
    popPart(head);
  }
  free(head);
}

Part* newPart(int x, int y){
  Part* part = malloc(sizeof(Part));
  part->x = x;
  part->y = y;

  return part;
}

Part* getTop(Part* head){
  Part* next = head;
  while(next->next != NULL){
    next = next->next;
  }
  return head->next;
}

void drawSnake(Canvas* canvas, Part* head){
  Part* next = head->next;
  int i = 0;
  while(next != NULL){
      setPixel(canvas,next->x,next->y,tailChar,BLACK,BG_BLACK);
    next = next->next;
    i++;
  }
  setPixel(canvas,head->next->x,head->next->y,headChar,BLUE,BG_BLACK);
}

void addSnake(Part* head,int nx, int ny){
  Part* next = head;
  Part* temp = head->next;
  head->next = newPart(nx,ny);
  head->next->next = temp;
}

void popPart(Part* head){
  Part* next = head;
  while(next->next->next != NULL){
    next = next->next;
  }
  Part* rem = next->next;
  next->next = NULL;
  free(rem);
}

void moveSnake(Part* head, int nx, int ny,int *add){
  if(*add == 0){
    popPart(head);
  }else{
    *add = 0;
  }

  addSnake(head,nx,ny);
}

void setDirection(char input,int *xd, int *yd){
    if(input == 'd' && *xd != -1){
      *xd = 1;
      *yd = 0;
    }
    if(input == 'a'&& *xd != 1){
      *xd = -1;
      *yd = 0;
    }
    if(input == 'w'&& *yd != 1){
      *xd = 0;
      *yd = -1;
    }
    if(input == 's'&& *yd != -1){
      *xd = 0;
      *yd = 1;
    }

}

void moveApple(Part* apple, int xmax,int ymax){
  int x = rand() % xmax-1;
  int y = rand() % ymax-1;

  apple->x=x;
  apple->y=y;
}

void exitGame(Canvas* canvas,Part* snakeHead, Part* apple, Part* apple2){
  freeSnake(snakeHead);
  free(apple);
  freeCanvas(canvas);
  exit(0);
}

int checkCollision(Part* snake){
  Part* next = snake->next;
  while(next != NULL){
    //check outside playfield
    if(next->x > canvas->width-1 || next->x < 0 || next->y > canvas->height-1 || next->y < 0){
      return 1;
    }
    //check all other parts of the snake
    Part* next1 = snake->next;
    while(next1 != NULL){
      if( next != next1 &&
          next->x==next1->x &&
          next->y == next1->y)
        return 1;
      next1 = next1->next;
    }
    next = next->next;
  }
  return 0;
}

int checkCollisionApple(Part* snake,int x, int y){
  Part* next = snake->next;
  while(next != NULL){
    if( next->x==x && next->y == y)
        return 1;
    next = next->next;
  }
  return 0;
}

int appleCollide(Part* head, Part* apple){
    //check if snake eats apple
    if(getTop(head)->x == apple->x && getTop(head)->y == apple->y){
      score++;
      moveApple(apple,canvas->width,canvas->height);
      int max = 10000;
      while(max > 0 && checkCollisionApple(head,apple->x,apple->y)){
        moveApple(apple,canvas->width,canvas->height);
        max --;
      }
      if(max < 1){
        // everyting is taken up?
      }
      return 1;
    }
    return 0;
}

void drawApple(Canvas* canvas, Part* apple){
    setPixel(canvas,apple->x,apple->y,appleChar,RED,BG_GREEN);
}

int main(){
  srand(time(NULL));   // Initialization, should only be called once.


  canvas = newCanvas(width,height,"  ",WHITE,BG_GREEN);
  canvas->x = (termWidth()/2)-canvas->width;
  canvas->y = 7;

  Canvas *scoreCanvas = newCanvas(width,3, "  ",WHITE,BG_BLACK);
  scoreCanvas->x = canvas->x;
  scoreCanvas->y = 1;
  setCenterText(scoreCanvas,scoreCanvas->width/2,0,"CNAKE",WHITE,BG_BLACK);
  setCenterText(scoreCanvas,scoreCanvas->width/2,1,"Speed: 1",WHITE,BG_BLACK);

  Part *head = newPart(4,2);
  addSnake(head,canvas->width/2,canvas->height/2);
  addSnake(head,canvas->width/2+1,canvas->height/2);


  Part *apple = newPart(5,5);
  Part *apple1 = newPart(8,8);

  int x=0,y=0;
  int xd=1,yd=0;
  int add = 0; // if true when the snake moves it will not remove the last part

  while(1){

    char input = getKeyPressed();

    if(input == 'q') exitGame(canvas,head,apple,apple1);

    setDirection(input,&xd,&yd);
    x += xd;
    y += yd;

    moveSnake(head,x,y,&add);


    //snake hits dies
    if(checkCollision(head)) {
      break;
    }

    add = appleCollide(head,apple);
    add = add ? 1 : appleCollide(head,apple1); // if add is 1 dont check appl1

    char scoreText[20];
    sprintf(scoreText,"Score: %d",score);
    setCenterText(scoreCanvas,scoreCanvas->width/2,2,scoreText,WHITE,BG_BLACK);

    //set snake and apple
    drawApple(canvas,apple);
    drawApple(canvas,apple1);
    drawSnake(canvas,head);

    //draw everything
    draw(canvas);
    drawBorder(canvas,0);
    draw(scoreCanvas);
    drawBorder(scoreCanvas,0);

    msleep(100);
    clearPixels(canvas);
  }
  exitGame(canvas,head,apple,apple1);


  return 0;
}
