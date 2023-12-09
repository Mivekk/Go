#include "Game.h"

int main()
{
#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle("Lukasz, Machutt, 193517");

	Game game;

	clrscr();
	gotoxy(1, 1);

	_setcursortype(_NORMALCURSOR);	

	return 0;
}