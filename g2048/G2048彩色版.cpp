#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#define S 4

int b[S][S],hghscr,nowscr;

void SetColor(int fore,int back){
	/*设置控制台文字颜色*/
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,fore|(back<<4));
}

// 单行左滑合并
bool ml(int r[]){
    bool m=0;
    int t[S]={0},k=0;
    for(int i=0;i<S;i++)if(r[i])t[k++]=r[i],m|=(k-1!=i);
    for(int i=0;i<S-1;i++)if(t[i]&&t[i]==t[i+1]){t[i]*=2;t[i+1]=0;m=1;i++;}
    k=0;
    for(int i=0;i<S;i++)if(t[i])t[k++]=t[i];
    for(int i=k;i<S;i++)t[i]=0;
    for(int i=0;i<S;i++)r[i]=t[i];
    return m;
}

// 数组反转
void rev(int a[],int l){
    int i=0,j=l-1,t;
    for(;i<j;i++,j--){
		t=a[i];
		a[i]=a[j];
		a[j]=t;
	}
}

// 方向移动
bool mv(int d){
    bool m=0;
    int t[S][S];
    if(d==0){
        for(int j=0;j<S;j++){
			for(int i=0;i<S;i++)
				t[j][i]=b[i][j];
			m|=ml(t[j]);
			for(int i=0;i<S;i++)
				b[i][j]=t[j][i];
		}
    }else if(d==1){
        for(int j=0;j<S;j++){
			for(int i=0;i<S;i++){
				t[j][i]=b[S-1-i][j];
			}
			m|=ml(t[j]);
			for(int i=0;i<S;i++){
				b[S-1-i][j]=t[j][i];
			}
		}
    }else if(d==2){
        for(int i=0;i<S;i++)
			m|=ml(b[i]);
    }else{
        for(int i=0;i<S;i++){
			rev(b[i],S);
			m|=ml(b[i]);
			rev(b[i],S);
		}
    }
    return m;
}

// 随机生成2/4
void gn(){
    int e[S*S][2],c=0;
    for(int i=0;i<S;i++)
		for(int j=0;j<S;j++)
			if(!b[i][j]){
				e[c][0]=i;
				e[c++][1]=j;
			}
    if(c){
		int idx=rand()%c;
		b[e[idx][0]][e[idx][1]]=(rand()%10?2:4);
	}
}

void init(){
    int x1=rand()%S,y1=rand()%S,x2,y2;
    b[x1][y1]=rand()%10?2:4;
    do{
		x2=rand()%S;
		y2=rand()%S;
	}while(x2==x1 && y2==y1);
    b[x2][y2]=rand()%10?2:4;
}

// 根据数字拿背景色
int GetBg(int n){
    switch(n){
        case 2:return 7;
        case 4:return 6;
        case 8:return 14;
        case 16:return 12;
        case 32:return 9;
        case 64:return 13;
        case 128:return 10;
        case 256:return 11;
        case 512:return 5;
        case 1024:return 3;
        case 2048:return 4;
        default:return 8;
    }
}

void pr(){
    system("cls");
    printf("============= 2048 =============\n");
    printf("w:up  s:down  a:left  d:right  q:quit\n\n");

    for(int i=0;i<S;i++){
        // 上边框
        for(int j=0;j<S;j++){
            SetColor(0,GetBg(b[i][j]));
            printf("      ");
        }
        SetColor(7,0);
        printf("\n");

        // 数字行
        for(int j=0;j<S;j++){
            SetColor(0,GetBg(b[i][j]));
            if(b[i][j])
                printf("%4d  ",b[i][j]);
            else
                printf("      ");
        }
        SetColor(7,0);
        printf("\n");

        // 下边框
        for(int j=0;j<S;j++){
            SetColor(0,GetBg(b[i][j]));
            printf("      ");
        }
        SetColor(7,0);
        printf("\n");
    }
}

bool win(){
    for(int i=0;i<S;i++)
        for(int j=0;j<S;j++)
            if(b[i][j]==2048)
				return 1;
    return 0;
}

bool lose(){
    for(int i=0;i<S;i++)
		for(int j=0;j<S;j++)
			if(!b[i][j])
				return 0;
    for(int i=0;i<S;i++)
		for(int j=0;j<S-1;j++)
			if(b[i][j]==b[i][j+1])
				return 0;
    for(int j=0;j<S;j++)
		for(int i=0;i<S-1;i++)
			if(b[i][j]==b[i+1][j])
				return 0;
    return 1;
}

int main(){
    char o;
    bool m=0;
    srand((unsigned int)time(0));
    init();
    // 隐藏光标
    CONSOLE_CURSOR_INFO ci={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&ci);

    while(1){
        m=0;
        pr();
        if(win()){
		    SetColor(7,0);
		    printf("\nWin!\n");
		    system("pause>nul");
		    exit(0);
		}
        if(lose()){
			SetColor(7,0);
			printf("\nLose!\n");
			system("pause>nul");
			exit(0);
		}
        o=_getch();
        switch(o){
            case'w':m=mv(0);break;
            case's':m=mv(1);break;
            case'a':m=mv(2);break;
            case'd':m=mv(3);break;
			case'W':{
                for(int i=0;i<S;i++)
                    for(int j=0;j<S;j++)
                        if(!b[i][j]){
							b[i][j]=2048;
							goto brk;
						}
				brk:break;
            }
            case'l':{
                int num=2;
                for(int i=0;i<S;i++)
                    for(int j=0;j<S;j++){
                        b[i][j]=num;
                        num*=2;
                        if(num>256){
							num=2;
						}
                        if(j>0 && b[i][j]==b[i][j-1]){
							num+=2;
						}if(i>0 && b[i][j]==b[i-1][j]){
							num+=2;
						}
                    }
                break;
            }
            case'q':
                SetColor(7,0);
                exit(0);
            default:
				continue;
        }
        if(m){
			gn();
		}
    }
    SetColor(7,0);
    return 0;
}