#include<windows.h>
#include<stdio.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
int b[3][3]={0};
int win();
int mv(int,int,int);
int mainLogic(int x,int y,int who){
    if(mv(x,y,who)) return win();
    return -1;
}
int mv(int x,int y,int p){
    if(x<0||x>2||y<0||y>2||b[x][y]!=0)
        return 0;
    b[x][y]=p;
    return 1;
}
int win(){
    bool f=1;
    for(int i=0;i<3;i++){
        if(b[i][0]==b[i][1]&&b[i][1]==b[i][2]&&b[i][0]!=0)return b[i][0];
        if(b[0][i]==b[1][i]&&b[1][i]==b[2][i]&&b[0][i]!=0)return b[0][i];
    }
    if(b[0][0]==b[1][1]&&b[1][1]==b[2][2]&&b[0][0]!=0)return b[0][0];
    if(b[0][2]==b[1][1]&&b[1][1]==b[2][0]&&b[0][2]!=0)return b[0][2];
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(b[i][j]==0)f=0;
    return f?3:0;
}
#define CELL 120
#define GAP 10
#define WND_W (CELL*3+GAP*4)
#define WND_H (CELL*3+GAP*4+40)
int nowPlayer=1;
void DrawBoard(HDC hdc){
    RECT r;
    HBRUSH hBr;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            int x=GAP+j*(CELL+GAP);
            int y=40+GAP+i*(CELL+GAP);
            SetRect(&r,x,y,x+CELL,y+CELL);
            hBr=CreateSolidBrush(RGB(245,245,245));
            FillRect(hdc,&r,hBr);
            DeleteObject(hBr);
            if(b[i][j]==1){
                SetTextColor(hdc,RGB(0,0,255));
                DrawText(hdc,"X",1,&r,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            }else if(b[i][j]==2){
                SetTextColor(hdc,RGB(255,0,0));
                DrawText(hdc,"O",1,&r,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            }
        }
    }
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
    if(msg==WM_PAINT){
        PAINTSTRUCT ps;
        HDC hdc=BeginPaint(hWnd,&ps);
        SetBkMode(hdc,TRANSPARENT);
        HFONT hFont=CreateFont(24,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
        SelectObject(hdc,hFont);
        char buf[50];
        sprintf(buf,"Current: Player %s",nowPlayer==1?"X (Blue)":"O (Red)");
        SetTextColor(hdc,RGB(0,0,0));
        TextOut(hdc,10,10,buf,strlen(buf));
        DrawBoard(hdc);
        DeleteObject(hFont);
        EndPaint(hWnd,&ps);
        return 0;
    }
    if(msg==WM_LBUTTONDOWN){
        int mx=LOWORD(lp);
        int my=HIWORD(lp)-40;
        int j=(mx-GAP)/(CELL+GAP);
        int i=(my-GAP)/(CELL+GAP);
        if(i>=0&&i<3&&j>=0&&j<3&&b[i][j]==0){
            int res=mainLogic(i,j,nowPlayer);
            if(res!=-1){
                nowPlayer=nowPlayer==1?2:1;
                InvalidateRect(hWnd,0,1);
                if(res==1||res==2){
                    char msg[50];
                    sprintf(msg,"Player %s Wins!",res==1?"X":"O");
                    MessageBox(hWnd,msg,"Game Over",0);
                    PostQuitMessage(0);
                }else if(res==3){
                    MessageBox(hWnd,"Draw Game!","Game Over",0);
                    PostQuitMessage(0);
                }
            }
        }
        return 0;
    }
    if(msg==WM_DESTROY){PostQuitMessage(0);return 0;}
    return DefWindowProc(hWnd,msg,wp,lp);
}
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hP,LPSTR cmd,int nShow){
    const char CLASS[]="TicTacToeGUI";
    WNDCLASS wc={0};
    wc.lpfnWndProc=WndProc;
    wc.hInstance=hInst;
    wc.lpszClassName=CLASS;
    wc.hCursor=LoadCursor(0,IDC_ARROW);
    RegisterClass(&wc);
    HWND hWnd=CreateWindow(CLASS,"Tic Tac Toe",WS_OVERLAPPED|WS_SYSMENU|WS_CAPTION,100,100,WND_W,WND_H,0,0,hInst,0);
    ShowWindow(hWnd,nShow);
    MSG m;
    while(GetMessage(&m,0,0,0)){TranslateMessage(&m);DispatchMessage(&m);}
    return 0;
}