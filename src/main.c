#include "msc.c"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 20
#define HEIGHT 20

typedef struct vector{
    int x,y;
} Vector;

typedef struct node{
    struct node *next;
    struct node *parent;
    Vector pos;
} Node;


int len = 0;
Node *head;

Vector apple;

Node *init(){
    Node *head = malloc(sizeof(Node));
    head->next = NULL;
    head->parent = NULL;
    head->pos.x = 0;
    head->pos.y = 0;
    return head;
}

Node *add(Node *head, int x, int y){
    Node *pnode = head;
    while(pnode->next!=NULL){
        pnode = pnode->next;
    }
    //add new move 
    Node *newMove = malloc(sizeof(Node));
    newMove->next = NULL;
    newMove->parent = pnode;
    newMove->pos.x = x;
    newMove->pos.y = y;
    pnode->next = newMove;
    len++;
    return newMove;
}

Node *move(Node *head, Vector direction){

    Node *new = add(head, direction.x, direction.y);
    if(new->parent != NULL){
        new->pos.x += new->parent->pos.x;
        new->pos.y += new->parent->pos.y;
    }
    return new;
}

Node *get(Node* head, int index){
    Node *pnode = head;
    int i = 0;
    while(pnode->next!=NULL){
        pnode = pnode->next;
        if(i == index) return pnode;
        i++;
    }
    return NULL;
}

void spaces(int spaces){
    for(int i = 0; i < spaces; i++){
        printf(" ");
    }
}
void chars(int spaces, char *c){
    for(int i = 0; i < spaces; i++){
        printf(c);
    }
}
void render(){
    system("clear");
    spaces(WIDTH-5);
    printf("Score:%d\n",len-3);
    int board[WIDTH*HEIGHT];
    for(int j = 0;j<HEIGHT*WIDTH;j++){
        board[j] = 0;
    }

    int y = 0;
    int x = 0;
    for(int j = 0;j<HEIGHT*WIDTH;j++){
        if(apple.x == x && apple.y == y){
            board[j] = 2;
        }
        for(int i = 0; i < len;i++){
            Node *tail = get(head, i);
            if(tail->pos.x == x && tail->pos.y == y){
                if( i == len-1 )
                board[j] = 3;
                else board[j] = 1;
            }
        }
        if(x == WIDTH-1){
            y++;
            x=-1;
        }
        x++;
    }
    
    int row = 0;
    chars(WIDTH*2+2,"▄");
    printf("\n█");
    for(int j = 0;j<HEIGHT*WIDTH;j++){
        if(board[j] == 1){
            printf("\033[0;32m██");
        }
        if(board[j] == 3){
            printf("\033[33;1m██");
        }
        if(board[j] == 0){
            printf("· ");
        }
        if(board[j] == 2){
            printf("\033[31;1m@ ");
        }
        printf("\033[0m");
        if(row == WIDTH-1){
             printf("█\n█");
             row=-1;
        }
        row ++;
    }
    chars(WIDTH*2,"▄");
    printf("█\n");
}

int main(){
    head = init();
    add(head, 1,1);
    add(head, 2,1);
    add(head, 3,1);
    add(head, 4,1);
    add(head, 5,1);
    add(head, 6,1);
    add(head, 7,1);
    add(head, 8,1);
    add(head, 9,1);
    
    apple.x = 10;
    apple.y = 5;
    int time = 100;
    Vector direction;
    direction.x = 0;
    direction.y = 1;

    while(1){

        if(kbhit()){
            char key = getchar();
            if(key == 'd' && direction.x != -1) {
                direction.x = 1;
                direction.y = 0;
            }
            if(key == 'a'&& direction.x != 1) {
                direction.x = -1;
                direction.y = 0;
            }
            if(key == 'w'&& direction.y != 1) {
                direction.x = 0;
                direction.y = -1;
            }
            if(key == 's'&& direction.y != -1) {
                direction.x = 0;
                direction.y = 1;
            }
        }
        time--;
        if(time == 0){
            time=100;
           //snake hit apple 
            if(head->parent != NULL && head->parent->pos.x == apple.x && head->parent->pos.y == apple.y){
                move(head, direction);
                srand(time(0));
                int x = (rand() % (WIDTH - 1 + 1)) + 0;
                int y = (rand() % (HEIGHT - 1 + 1)) + 0;
                apple.x = x;
                apple.y = y;

            }else{
                Node *new = move(head, direction);
                len--;
                //remove old
                if(head->next->next != NULL){
                    Node *old = head->next;
                    head->next = head->next->next;
                    head->parent = new;
                    free(old);
                }
            }
            for(int i = 0;i<len;i++){
                Node * head1 = get(head,i);
                if(head1->pos.x > WIDTH-1 || head1->pos.x < 0 || 
                        head1->pos.y > HEIGHT-1 || head1->pos.y < 0){
                    return 0;
                }
                for(int j = 0;j<len;j++){
                    Node * head2 = get(head,j);
                    if(head1 != head2){
                        if(head1->pos.x == head2->pos.x && head1->pos.y == head2->pos.y){
                            return 0;
                        }
                    }
                }
            }

        }
        msleep(10);
        render();
    }

    return 0;
}


