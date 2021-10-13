#include<windows.h>
#include<stdio.h>
#include<time.h>

#define screen_x 80
#define screen_y 25
#define scount 80

HANDLE wHnd;
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD star[scount];
int color = 7;
int oldcolor = 7;
int score = 0;
bool play = true;

HANDLE rHnd;
DWORD fdwMode;

DWORD numEvents = 0;
DWORD numEventsRead = 0;

int setConsole(int x, int y){
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void clear_buffer() {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			if (consoleBuffer[x + screen_x * y].Char.AsciiChar != ' ' &&
				consoleBuffer[x + screen_x * y].Char.AsciiChar != '<' &&
				consoleBuffer[x + screen_x * y].Char.AsciiChar != '-' &&
				consoleBuffer[x + screen_x * y].Char.AsciiChar != '0' &&
				consoleBuffer[x + screen_x * y].Char.AsciiChar != '>' ) {
				consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
				consoleBuffer[x + screen_x * y].Attributes = 7;
			}
		}
	}
}

void init_star() {
	for (int i = 0; i < scount; i++) {
		star[i].X = rand() % screen_x;
		star[i].Y = rand() % screen_y;
	}
}

void checkstar(int x,int y,int i) {
	if (consoleBuffer[x + screen_x * y].Char.AsciiChar != ' ') {
		score++;
		star[i].X = rand() % screen_x;
		star[i].Y = rand() % screen_y;
	}
}

void starfall() {
	int i;
	for (i = 0; i < scount; i++) {
		if (star[i].Y >= screen_y - 1) {
			star[i] = { (rand() % screen_x),1 };
		}
		else {
			star[i] = { star[i].X,star[i].Y + 1 };
		}
	}
}

void fill_star_to_buffer() {
	for (int i = 0; i < scount ; i++) {
		if (consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar == '<'|| 
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar == '-'||
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar == '0'||
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar == '>') {
			score++;
			if (score >= 10) play=false;
			star[i].X = rand() % screen_x;
			star[i].Y = rand() % screen_y;
		}
		consoleBuffer[star[i].X + screen_x*star[i].Y].Char.AsciiChar = '*';
	}
}

void fill_buffer_to_console(){
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,&windowSize);
}

int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}

void drawShip(int x, int y) {
	/*
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	printf("<-0->");
	*/
	consoleBuffer[x + screen_x * y].Char.AsciiChar = '<';
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = '-';
	consoleBuffer[x + 2 + screen_x * y].Char.AsciiChar = '0';
	consoleBuffer[x + 3 + screen_x * y].Char.AsciiChar = '-';
	consoleBuffer[x + 4 + screen_x * y].Char.AsciiChar = '>';

	consoleBuffer[x + screen_x * y].Attributes = color;
	consoleBuffer[x + 1 + screen_x * y].Attributes = color;
	consoleBuffer[x + 2 + screen_x * y].Attributes = color;
	consoleBuffer[x + 3 + screen_x * y].Attributes = color;
	consoleBuffer[x + 4 + screen_x * y].Attributes = color;
}

void clearShip(int x, int y) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 2 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 3 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 4 + screen_x * y].Char.AsciiChar = ' ';

	consoleBuffer[x + screen_x * y].Attributes = 7;
	consoleBuffer[x + 1 + screen_x * y].Attributes = 7;
	consoleBuffer[x + 2 + screen_x * y].Attributes = 7;
	consoleBuffer[x + 3 + screen_x * y].Attributes = 7;
	consoleBuffer[x + 4 + screen_x * y].Attributes = 7;
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void ChangeColor() {
	color = rand() % 15 + 1;
}

int main()
{
	setcursor(0);
	init_star();
	srand(time(NULL));
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);	
	setMode();
	int oldposx=0,oldposy=0;

	while (play) {		
		starfall();
		clear_buffer();
		fill_star_to_buffer();
		fill_buffer_to_console();
		//Sleep(200);

		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

			for (DWORD i = 0; i < numEventsRead; ++i) {

				int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
				int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;

				if (eventBuffer[i].EventType == KEY_EVENT) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						play = false;
					}
					if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c') {
						ChangeColor();
					}	
				}
							
				if (eventBuffer[i].EventType == MOUSE_EVENT) {
					if (eventBuffer[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
						if (posx != oldposx || posy != oldposy )
						{
							clearShip(oldposx, oldposy);
						}
						drawShip(posx, posy);
						oldposx = posx;
						oldposy = posy;
					}
					if (eventBuffer[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						ChangeColor();
					}
				}
			}
			delete[] eventBuffer;
		}
		Sleep(200);
	}
	return 0;
}
