#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

#define screen_x 80
#define screen_y 25
HANDLE wHnd;
SMALL_RECT windowSize = { 0,0,screen_x-1,screen_y-1};

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	return 0;
}

void drawShip(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	cout << " <-0-> ";
}
void eraseShip(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	cout << "       ";
}
/*
void gotoxy(int x,int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
*/
int main() {
	setConsole(screen_x, screen_y);
	char ch = ' ';
	int x = 38, y = 20;
	drawShip(x,y);
	do {
		if (_kbhit()) {
			ch = _getch();
				if (ch == 'a') {
					if (x == 1) drawShip(x, y);
					else drawShip(--x, y); 
				}
				if (ch == 'd') {
					if (x == 73)drawShip(x, y);
					else drawShip(++x, y); 
				}
				if (ch == 'w') {
					eraseShip(x, y);
					if (y == 0) drawShip(x, y);
					else drawShip(x, --y);
				}
				if (ch == 's') {
					eraseShip(x, y);
					drawShip(x, ++y);
				}
			fflush(stdin);
		}
		Sleep(100);
	} while (ch != x);
	return 0;
}