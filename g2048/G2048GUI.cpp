#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define S 4
#define CELL_SIZE 100
#define WIN_W (CELL_SIZE*S+40)
#define WIN_H (CELL_SIZE*S+80)

int b[S][S];

// 你的原版滑动逻辑 一字不改
bool ml(int r[]) {
	bool m=0;
	int t[S]= {0},k=0;
	for(int i=0; i<S; i++)if(r[i])t[k++]=r[i],m|=(k-1!=i);
	for(int i=0; i<S-1; i++)if(t[i]&&t[i]==t[i+1]) {
			t[i]*=2;
			t[i+1]=0;
			m=1;
			i++;
		}
	k=0;
	for(int i=0; i<S; i++)if(t[i])t[k++]=t[i];
	for(int i=k; i<S; i++)t[i]=0;
	for(int i=0; i<S; i++)r[i]=t[i];
	return m;
}

void rev(int a[],int l) {
	int i=0,j=l-1,t;
	for(; i<j; i++,j--) {
		t=a[i];
		a[i]=a[j];
		a[j]=t;
	}
}

bool mv(int d) {
	bool m=0;
	int t[S][S];
	if(d==0) {
		for(int j=0; j<S; j++) {
			for(int i=0; i<S; i++)t[j][i]=b[i][j];
			m|=ml(t[j]);
			for(int i=0; i<S; i++)b[i][j]=t[j][i];
		}
	} else if(d==1) {
		for(int j=0; j<S; j++) {
			for(int i=0; i<S; i++)t[j][i]=b[S-1-i][j];
			m|=ml(t[j]);
			for(int i=0; i<S; i++)b[S-1-i][j]=t[j][i];
		}
	} else if(d==2) {
		for(int i=0; i<S; i++)m|=ml(b[i]);
	} else {
		for(int i=0; i<S; i++) {
			rev(b[i],S);
			m|=ml(b[i]);
			rev(b[i],S);
		}
	}
	return m;
}

void gn() {
	int e[S*S][2],c=0;
	for(int i=0; i<S; i++)
		for(int j=0; j<S; j++)
			if(!b[i][j]) {
				e[c][0]=i;
				e[c++][1]=j;
			}
	if(c) {
		int idx=rand()%c;
		b[e[idx][0]][e[idx][1]]=(rand()%10?2:4);
	}
}

void init() {
	int x1=rand()%S,y1=rand()%S,x2,y2;
	b[x1][y1]=rand()%10?2:4;
	do {
		x2=rand()%S;
		y2=rand()%S;
	} while(x2==x1&&y2==y1);
	b[x2][y2]=rand()%10?2:4;
}

bool win() {
	for(int i=0; i<S; i++)
		for(int j=0; j<S; j++)
			if(b[i][j]==2048) return 1;
	return 0;
}

bool lose() {
	for(int i=0; i<S; i++)
		for(int j=0; j<S; j++)
			if(!b[i][j]) return 0;
	for(int i=0; i<S; i++)
		for(int j=0; j<S-1; j++)
			if(b[i][j]==b[i][j+1]) return 0;
	for(int j=0; j<S; j++)
		for(int i=0; i<S-1; i++)
			if(b[i][j]==b[i+1][j]) return 0;
	return 1;
}

// 根据数字取画刷颜色
COLORREF GetColor(int n) {
	switch(n) {
		case 2:
			return RGB(238,228,218);
		case 4:
			return RGB(237,224,200);
		case 8:
			return RGB(242,177,121);
		case 16:
			return RGB(245,149,99);
		case 32:
			return RGB(246,124,95);
		case 64:
			return RGB(246,94,59);
		case 128:
			return RGB(237,207,114);
		case 256:
			return RGB(237,204,97);
		case 512:
			return RGB(237,200,80);
		case 1024:
			return RGB(237,197,63);
		case 2048:
			return RGB(237,194,46);
		default:
			return RGB(205,193,180);
	}
}

// 绘图
void DrawBoard(HDC hdc) {
	int x0=20,y0=60;
	for(int i=0; i<S; i++) {
		for(int j=0; j<S; j++) {
			int x=x0+j*CELL_SIZE;
			int y=y0+i*CELL_SIZE;
			HBRUSH hBrush=CreateSolidBrush(GetColor(b[i][j]));
			HGDIOBJ hOld=SelectObject(hdc,hBrush);
			Rectangle(hdc,x,y,x+CELL_SIZE,y+CELL_SIZE);
			SelectObject(hdc,hOld);
			DeleteObject(hBrush);

			if(b[i][j]!=0) {
				char buf[20];
				sprintf(buf,"%d",b[i][j]);
				SetBkMode(hdc,TRANSPARENT);
				SetTextColor(hdc,RGB(0,0,0));
				RECT rc= {x,y,x+CELL_SIZE,y+CELL_SIZE};
				DrawText(hdc,buf,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
	switch(msg) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hwnd,&ps);
			DrawBoard(hdc);
			EndPaint(hwnd,&ps);
			return 0;
		}
		case WM_KEYDOWN: {
			bool ok=false;
			switch(wParam) {
				case VK_UP:
					ok=mv(0);
					break;
				case VK_DOWN:
					ok=mv(1);
					break;
				case VK_LEFT:
					ok=mv(2);
					break;
				case VK_RIGHT:
					ok=mv(3);
					break;
				// 作弊键 W L
				case 'W':
				case 'w': {
					for(int i=0; i<S; i++) {
						for(int j=0; j<S; j++) {
							if(b[i][j]==0) {
								b[i][j]=2048;
								goto endw;
							}
						}
					}
endw:
					InvalidateRect(hwnd,NULL,TRUE);
					return 0;
				}
				case 'L':
				case 'l': {
					int num=2;
					for(int i=0; i<S; i++) {
						for(int j=0; j<S; j++) {
							b[i][j]=num;
							num*=2;
							if(num>256) num=2;
							if(j>0&&b[i][j]==b[i][j-1]) num+=2;
							if(i>0&&b[i][j]==b[i-1][j]) num+=2;
						}
					}
					InvalidateRect(hwnd,NULL,TRUE);
					return 0;
				}
			}
			if(ok) gn();
			InvalidateRect(hwnd,NULL,TRUE);

			if(win()) {
				MessageBox(hwnd,"Win!","2048",MB_OK);
				PostQuitMessage(0);
			}
			if(lose()) {
				MessageBox(hwnd,"Lose!","2048",MB_OK);
				PostQuitMessage(0);
			}
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow) {
	srand((unsigned)time(NULL));
	init();

	const TCHAR CLASS_NAME[]=TEXT("2048GUI");
	WNDCLASS wc= {0};
	wc.lpfnWndProc=WndProc;
	wc.hInstance=hInst;
	wc.lpszClassName=CLASS_NAME;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	RegisterClass(&wc);

	HWND hwnd=CreateWindow(CLASS_NAME,TEXT("GUI.2048"),WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
	                       CW_USEDEFAULT,CW_USEDEFAULT,WIN_W,WIN_H,NULL,NULL,hInst,NULL);

	ShowWindow(hwnd,nShow);
	UpdateWindow(hwnd);

	MSG m;
	while(GetMessage(&m,NULL,0,0)>0) {
		TranslateMessage(&m);
		DispatchMessage(&m);
	}
	return 0;
}