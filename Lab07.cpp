#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<thread>

using namespace std;

void setcolor(int, int);

void drawShip(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(10, 12);
	cout << " <-0-> ";
}
void drawBullet(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(7, 0);
	cout << "o";
	//Beep(700, 80);
}
void drawRand(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(14, 0);
	cout << "*";
}
void eraseRand(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(0, 0);
	cout << " ";
}
void eraseBullet(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(0, 0);
	cout << " ";
}
void eraseShip(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(0, 0);
	cout << "       ";
}
void showScore(int score) {
	COORD c = { 64,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	setcolor(7, 5);
	cout << "  SCORE : " << score <<"  ";
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}

int main() {
	setcursor(0);
	char ch = ' ';
	int x = 38, y = 20;
	int x2[5], y2[5];
	int bulletTotal = 0;
	int i = 0;
	bool bullet[5] = { 0,0,0,0,0 };
	int way = 0;
	int score = 0;
	srand(time(NULL));
	drawShip(x, y);
	showScore(score);
	for (int i = 0; i <= 20; i++) drawRand(rand() % 61 + 10 , rand() % 4 + 2 );
	do {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a' || ch == 'A') { way = -1; }
			if (ch == 'd' || ch == 'D') { way = 1; }
			if (ch == 's' || ch == 'S') { way = 0; }
			if (ch == ' ' && bulletTotal < 5) {
				bullet[i] = 1;
				x2[i] = x + 3; y2[i] = y - 1;
				i++;
				thread q(Beep, 500, 100);
				q.detach();
				if (i == 5) i = 0;
				bulletTotal++;
			}
			fflush(stdin);
		}
		if (way == -1 && x >= 0) {
			eraseShip(x, y);
			if (x == 0) drawShip(x, y);
			else drawShip(--x, y);
		}
		if (way == 1 && x <= 73) {
			eraseShip(x, y);
			if (x == 73) drawShip(x, y);
			else drawShip(++x, y);
		}
		Sleep(100);
		for (int i = 0; i < 5; i++) {
			if (bullet[i] == 1) {
				eraseBullet(x2[i], y2[i]);
				if (y2[i] > 1) {
					if (cursor(x2[i], y2[i] - 1) == '*') {
						eraseBullet(x2[i], y2[i]);
						eraseRand(x2[i], y2[i] - 1);
						score++;
						showScore(score);
						Beep(900, 100);
						drawRand(rand() % 61 + 10, rand() % 4 + 2);
						bullet[i] = 0;
						bulletTotal--;
					}
					else drawBullet(x2[i], --y2[i]);
				}
				else {
					eraseBullet(x2[i], y2[i]);
					bullet[i] = 0;
					bulletTotal--;
				}
			}
		}
	} while (ch != 'x');
	return 0;
}