#pragma once
#include "conio2.h"
#include "Pair.h"
#include "Cursor.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>

class Board
{
private:
	int m_setCustomBoardSize();
	void m_drawHorizontalLine(Pair pos, bool up);
	void m_setBoardSize();
	void m_invalidBoardSize();

public:
	int sideLength;
	int xOffset;
	int yOffset;
	int maxVisibleSize;
	char** status;
	char** finalStatus;
	char** lastBlackStatus;
	char** lastWhiteStatus;

public:
	Board();
	Board(int boardSize);
	~Board();
	
	void boardInit();
	void drawBoard();
	void clearLastPosition(Cursor cursor);
};

