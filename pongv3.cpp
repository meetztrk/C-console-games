#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

typedef struct{
	int layoutx,layouty,playersize;		        //oyun ayarlari
    double speed;
}game;

typedef struct{
	char *head,*cur;
    bool active;
    int position;						        //oyuncu ve pozisyon bilgisi
}player;

typedef struct{
	int vx,vy;
    int **head,**cur;
    int position;				                //top ozellikleri
}table;

const char *screen;								//oyun ekrani icin string
char *scr;										//oyun ekrani icin degisken string
const char *borders;							//oyun sınırları icin string
game *settings;									//oyun ayarlari
player *player1;								//oyuncu pointer
player *player2;
table *b;

void CreateGame();
void GameSet(int,int,int,double);
void PrintScr();
void CreateScr();
void ScrollPlayer(char);
bool Action();
int mygetch();
void NewGame();

int mygetch() {
  struct termios oldt,newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

void PrintScr(){
    system("clear");
    printf(borders);
    printf(screen,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20);
    printf(borders);
}

void CreateGame(){
    int playerarray=(settings->layouty-2)*2-(settings->playersize);
	player1=(player*) malloc(sizeof(player));
    player1->head=(char*) malloc(sizeof(char)*playerarray);
    player2=(player*) malloc(sizeof(player));
    player2->head=(char*) malloc(sizeof(char)*playerarray);
    for(int i=0;i<playerarray;i++){
        if(i<(settings->layouty-2)&&i>=(settings->layouty-2-settings->playersize)){
            player1->head[i]=0x5d;
            player2->head[i]=0x5b;
        }
        else{
            player1->head[i]=0x20;
            player2->head[i]=0x20;
        }
    }
    player1->cur=player1->head+((settings->layouty-settings->playersize-2)/2);
    player2->cur=player2->head+((settings->layouty-settings->playersize-2)/2);
    player1->active=0;
    player2->active=1;
    player1->position=player1->head+settings->layouty-(player1->cur+settings->playersize+2);
    player2->position=player2->head+settings->layouty-(player2->cur+settings->playersize+2);
    b=(table*) malloc(sizeof(table));
    b->head=(int**) malloc(sizeof(int*)*((settings->layouty-3)*2+1));
    for(int i=0;i<((settings->layouty-3)*2+1);i++){
        b->head[i]=(int*) malloc(sizeof(int)*2);
        b->head[i][0]=settings->layoutx-4;
        b->head[i][1]=0;
    }
    b->head[settings->layouty-3][0]=(settings->layoutx-5)/2;
    b->head[settings->layouty-3][1]=(settings->layoutx-5)/2;
    b->cur=&b->head[(settings->layouty-3)/2];
    b->position=b->head+settings->layouty-(b->cur+3);
    b->vx=2;
    b->vy=1;
    char *border=(char*) malloc(sizeof(char)*settings->layoutx+2);
    border[0]=0x2b;
    for(int i=1;i<settings->layoutx-1;i++){
        border[i]=0x2d;
    }
    border[settings->layoutx-1]=0x2b;
    border[settings->layoutx]='\n';
    border[settings->layoutx+1]=0x00;
    borders=border;
    scr=(char*) malloc(sizeof(char)*(settings->layouty-2)*9+16);
    screen=scr;
}

void GameSet(int lyx,int lyy,int pysize,double spd){
    settings = (game*) malloc(sizeof(game));
    settings->layoutx=lyx;
    settings->layouty=lyy;
    settings->playersize=pysize;
    settings->speed=spd;
}

void CreateScr(){
    int cursor=0;
    for(int i=0;i<settings->layouty-2;i++){
        scr[cursor++]=0x7c;
        scr[cursor++]=player1->cur[i];
        if(b->cur[i][0]>0){
            scr[cursor++]=0x25;
            if(b->cur[i][0]>9){
                scr[cursor++]=b->cur[i][0]/10+48;
            }
            scr[cursor++]=b->cur[i][0]%10+48;
            scr[cursor++]=0x63;
        }
        if(b->cur[i]==b->head[settings->layouty-3]){
            scr[cursor++]=0x40;
            if(b->cur[i][1]>0){
                scr[cursor++]=0x25;
                if(b->cur[i][1]>9){
                    scr[cursor++]=b->cur[i][1]/10+48;
                }
                scr[cursor++]=b->cur[i][1]%10+48;
                scr[cursor++]=0x63;
            }
        }
        scr[cursor++]=player2->cur[i];
        scr[cursor++]=0x7c;
        scr[cursor++]='\n';
    }
    scr[cursor]=0x00;
}

void NewGame(){
    int x,y,ps;
    printf("game space width,height,playersize:");
    scanf(" %d %d %d",&x,&y,&ps);
    GameSet(x,y,ps,1);
    CreateGame();
}

void ScrollPlayer(char ch){
    switch(ch){
        case 0x77:
        case 0x57:
            if(player1->active && player1->cur[0]==0x20){
                player1->cur++;
            }
            break;
        case 0x73:
        case 0x53:
            if(player1->active && player1->cur>player1->head){
                player1->cur--;
            }
            break;
        case 0x41:
            if(player2->active && player2->cur[0]==0x20){
                player2->cur++;
            }
            break;
        case 0x42:
            if(player2->active && player2->cur>player2->head){
                player2->cur--;
            }
            break;
        case 'q':
            NewGame();
            break;
        default:
            break;
    }
    player1->position=player1->head+settings->layouty-(player1->cur+settings->playersize+2);
    player2->position=player2->head+settings->layouty-(player2->cur+settings->playersize+2);
}

bool Action(){
    bool dv=true;
    b->head[settings->layouty-3][0]+=(settings->speed*b->vx);
    if(b->head[settings->layouty-3][0]<0) b->head[settings->layouty-3][0]=0;
    if(b->head[settings->layouty-3][0]>settings->layoutx-5) b->head[settings->layouty-3][0]=settings->layoutx-5;
    b->head[settings->layouty-3][1]=settings->layoutx-5-b->head[settings->layouty-3][0];
    b->cur=b->cur+(int)(settings->speed*b->vy);
    b->position=b->head+settings->layouty-(b->cur+3);
    if(b->head[settings->layouty-3][0]<=0){
        b->vx=-b->vx;
        if(player1->position<=b->position && player1->position+settings->playersize>=b->position){
            player2->active=1;
            player1->active=0;
        }
        else dv=false;
    }
    if(b->head[settings->layouty-3][1]<=0){
        b->vx=-b->vx;
        if(player2->position<=b->position && player2->position+settings->playersize>=b->position){
            player2->active=0;
            player1->active=1;
        }
        else dv=false;
    }
    if(b->cur>b->head+settings->layouty-4||b->cur==b->head){
        b->vy=-b->vy;
    }
    return dv;
}

int main(){
    char ch;
    GameSet(75,19,5,1);
    CreateGame();
    CreateScr();
    PrintScr();
    while(Action()){
        //Sleep(33);
        ch=mygetch();
        ScrollPlayer(ch);
        CreateScr();
        PrintScr();
    }
    printf("GAME OVER!!");
    
    return 0;
}