#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

int mainmatrix[4][4];               //this matrix show on screen
int rotatematrix[4][4];             //this is using temp for rotate process
int syncmatrix[4][4];
int undomatrix[4][4];
int spacearea[16];                  //this is keeping blank area
int score=0;                        //game score


void GUI(); 			//Print mainmatrix and score
int mygech();                
int BlankArea();            //Find blank area in mainmatrix
void Spawn();               //Spawn 2 or 4 on blank area
bool DelSpace();            
bool Swipe();
void Rotate();
void Sync();
void UndoSync();
void Undo();
bool Action(char);
bool Control();             //Control for GAME OVER



void GUI(){
    system("clear");                           //Clear screen 
	printf("Score: %d\n+------+------+------+------+\n",score);
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(mainmatrix[i][j]>0) printf("|%6d",mainmatrix[i][j]);
			else printf("|      ");
		}
		printf("|\n|      |      |      |      |\n+------+------+------+------+\n");
	}
}

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

int BlankArea(){
    int s=0;                            //How many blank area in mainmatrix
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(mainmatrix[i][j]==0){
                spacearea[s]=i*4+j;     //Blank areas position save
                s++;
            }
        }
    }
    return s;                           //return how many blank
}

void Spawn(){
    int s=BlankArea();
    if(s>0) mainmatrix[0][spacearea[rand()%s]]=((rand()%2+1)*2);        //Chose random position and give 2 or 4 in random cell
}

bool DelSpace(){
    int counter;
    bool r=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            counter=0;
            if(mainmatrix[i][j]==0){
                counter=i+1;
                while(counter<3&&mainmatrix[counter][j]==0){
                    counter++;
                }
                if(mainmatrix[counter][j]!=0){
                    mainmatrix[i][j]=mainmatrix[counter][j];
                    mainmatrix[counter][j]=0;
                    r=1;
                }
            }
        }
    }
    return r;
}

bool Swipe(){
    bool r=0;
    if(DelSpace()) r=1;
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            if(mainmatrix[i][j]!=0&&mainmatrix[i][j]==mainmatrix[i+1][j]){
                mainmatrix[i][j]*=2;
                score+=mainmatrix[i][j];
                mainmatrix[i+1][j]=0;
                r=1;
            }
        }
    }
    DelSpace();
    return r;
}

void Rotate(){
    for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			rotatematrix[i][j]=mainmatrix[3-j][i];
		}
	}
	for(int i=0;i<16;i++){
		mainmatrix[0][i]=rotatematrix[0][i];
	}
}

void Sync(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            syncmatrix[i][j]=mainmatrix[i][j];
        }
    }
}

void UndoSync(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            undomatrix[i][j]=syncmatrix[i][j];
        }
    }
}

void Undo(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            mainmatrix[i][j]=undomatrix[i][j];
        }
    }
}

bool Action(char act){
    bool r=0;
    switch (act){
        case 'w':
            Sync();
            if(Swipe()){
                UndoSync();
                Spawn();
                r=1;
            }
            break;
        case 'a':
            Sync();
            Rotate();
            if(Swipe()){
                UndoSync();
                Spawn();
                r=1;
            }
            Rotate();
            Rotate();
            Rotate();
            break;
        case 's':
            Sync();
            Rotate();
            Rotate();
            if(Swipe()){
                UndoSync();
                Spawn();
                r=1;
            }
            Rotate();
            Rotate();
            break;
        case 'd':
            Sync();
            Rotate();
            Rotate();
            Rotate();
            if(Swipe()){
                UndoSync();
                Spawn();
                r=1;
            }
            Rotate();
            break;
        case 'z':
            Undo();
            r=1;
            break;
        default:
            break;
    }
    return r;
}

bool Control(){
    bool r=0;
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            if(mainmatrix[i][j]==mainmatrix[i][j+1]) r++;
        }
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            if(mainmatrix[i][j]==mainmatrix[i+1][j]) r++;
        }
    }
    return r;
}

int main(){
    char act;
    bool gm=0;
    Spawn();
    Spawn();
    GUI();
    while(!gm){
        act=mygetch();
        if(Action(act)) {
            GUI();
        }
        else{
            if(BlankArea()==0){
                GUI();
                printf("\nThe job may be overworked\n");
                gm=(!Control());
            }
        }
    }
    printf("GAME OVER!!!");
    
    return 0;
}
