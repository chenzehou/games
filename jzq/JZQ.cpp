#include<stdio.h>
#include<conio.h>
#include<windows.h>
#define getch _getch
#define kbhit _kbhit
int b[3][3]={0};
void pr();
int win();
int mv(int,int,int);
int main(){
    int x,y,who=1,winn=0;
	SetConsoleOutputCP(CP_UTF8);
    system("title JingZiQi");
    do{
        nxt:system("cls");
        pr();
        printf("玩家%d (%c) 请输入坐标(1-3 1-3,空格分隔):",who,((who==1)?'X':'O'));
        scanf("%d%d",&x,&y);
        x--;y--;
		if(x<0 || y<0 || x>2 || y>2){
			printf("输入超出棋盘范围,请重新输入!\a");
			getch();
			goto nxt;
		}
        if(mv(x,y,who)){
            winn=win();
            who=who==1?2:1;
        }else{
            printf("位置非空!按任意键以重新输入...\a");
            getch();
            goto nxt;
        }
    }while(winn==0);
    system("cls");
    pr();
    if(winn==3)printf("平局!\n");
    else printf("玩家%d赢了!\a\n",winn);
    system("pause");
    return 0;
}
void pr(){
    printf("============== 井字棋 ==============\n");
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("  %c  ", b[i][j]==0?'-':(b[i][j]==1?'X':'O'));
            if(j<2){
            	printf("|");
			}
        }
        if(i<2){
			printf("\n-----+-----+-----\n");
		}
    }
    printf("\n");
}
int mv(int x,int y,int p){
    if(x<0 || x>2 || y<0 || y>2 || b[x][y]!=0){
    	return 0;
	}
    b[x][y]=p;
    return 1;
}
int win(){
	bool f=1;
    for(int i=0;i<3;i++){
        if(b[i][0]==b[i][1] && b[i][1]==b[i][2] && b[i][0]!=0)return b[i][0];
        if(b[0][i]==b[1][i] && b[1][i]==b[2][i] && b[0][i]!=0)return b[0][i];
    }
    if(b[0][0]==b[1][1] && b[1][1]==b[2][2] && b[0][0]!=0)return b[0][0];
    if(b[0][2]==b[1][1] && b[1][1]==b[2][0] && b[0][2]!=0)return b[0][2];
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(b[i][j]==0)f=0;
    return f?3:0;
}