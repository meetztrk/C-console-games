#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

typedef struct{
    char *start,*cur,*finish;
    bool active;
}player;

int PrintScr(player *py,player *py2){
    clrscr();
    //printf("start:%p\tcur:%p\tfinish:%p",py->start,py->cur,py->finish);
    for(int i=0;i<16;i++){
        printf("%c%61c\n",py->cur[i],py2->cur[i]);
    }

}

void ScrollPlayer(player *py,player *py2,char ch){
    switch (ch){
        case 'w':
            if(py->active && py->cur+16<py->finish){
                py->cur++;
            }
            break;
        case 's':
            if(py->active && py->cur>py->start){
                py->cur--;
            }
            break;
        case 'A':
            if(py2->active && py2->cur+16<py2->finish){
                py2->cur++;
            }
            break;
        case 'B':
            if(py2->active && py2->cur>py2->start){
                py2->cur--;
            }
            break;
        default:
            break;
    }
}

int main(){
    char ch;
    int a=0;
    player *player1=(player*) malloc(sizeof(player));
    player1->start="            ||||            ";
    player1->cur=player1->start;
    player1->finish=player1->start+28;
    player *player2=(player*) malloc(sizeof(player));
    player1->active=1;
    player2->start="            ||||            ";
    player2->cur=player2->start;
    player2->finish=player2->start+28;
    player2->active=1;
    printf("basladi\n");
    while(1){
        ch=getch();
        ScrollPlayer(player1,player2,ch);
        PrintScr(player1,player2);
    }

    return 0;
}
