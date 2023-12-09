#include "Board.h"
#include "Input.h"

Board::Board() 
	: sideLength(-1), xOffset(0), yOffset(0), 
	status(nullptr), finalStatus(nullptr), 
	lastBlackStatus(nullptr), lastWhiteStatus(nullptr), maxVisibleSize(0)
{
}

Board::Board(int boardSideLength)
	: status(nullptr), finalStatus(nullptr), xOffset(0), yOffset(0),
	lastBlackStatus(nullptr), lastWhiteStatus(nullptr)
{
	this->sideLength = boardSideLength;

	clrscr();
	boardInit();
}

Board::~Board()
{
	for (int i = 0; i < sideLength; i++) {
		delete[] status[i];
	}
	delete[] status;

	for (int i = 0; i < sideLength; i++) {
		delete[] finalStatus[i];
	}
	delete[] finalStatus;

	for (int i = 0; i < sideLength; i++) {
		delete[] lastBlackStatus[i];
	}
	delete[] lastBlackStatus;

	for (int i = 0; i < sideLength; i++) {
		delete[] lastWhiteStatus[i];
	}
	delete[] lastWhiteStatus;
}

void Board::boardInit()
{
	m_setBoardSize();

	clrscr();
	if (sideLength < 1) {
		gotoxy(1, 1);
		textcolor(RED);
		cputs("ERROR: BOARD SIZE < 1");
		return;
	}
	status = new char* [sideLength];
	for (int i = 0; i < sideLength; i++) {
		status[i] = new char[sideLength];
	}
	finalStatus = new char* [sideLength];
	for (int i = 0; i < sideLength; i++) {
		finalStatus[i] = new char[sideLength];
	}
	lastBlackStatus = new char* [sideLength];
	for (int i = 0; i < sideLength; i++) {
		lastBlackStatus[i] = new char[sideLength];
	}
	lastWhiteStatus = new char* [sideLength];
	for (int i = 0; i < sideLength; i++) {
		lastWhiteStatus[i] = new char[sideLength];
	}
	
	for (int i = 0; i < sideLength; i++) {
		for (int j = 0; j < sideLength; j++) {
			status[i][j] = 'e';
		}
	}
}

void Board::clearLastPosition(Cursor cursor)
{
	cputs("\b ");
	gotoxy(wherex() - 1, wherey());
	if (status[cursor.realPos.y - BOARD_STARTING_Y - 1]
		[(cursor.realPos.x - DETERMINE_BOARD) / 2 - 1] == 'b') {
		textcolor(BLACK);
		textbackground(BLACK);
	}
	else if (status[cursor.realPos.y - BOARD_STARTING_Y - 1]
		[(cursor.realPos.x - DETERMINE_BOARD) / 2 - 1] == 'w') {
		textcolor(WHITE);
		textbackground(WHITE);
	}
	else {
		textcolor(DEFAULT_TEXT_COLOR);
		textbackground(BLACK);
	}
	putch(197);
}

void Board::drawBoard()
{
	textcolor(DEFAULT_TEXT_COLOR);
	textbackground(BLACK);
	int startingPos = DETERMINE_BOARD;

	if (!yOffset) {
		m_drawHorizontalLine({ startingPos, BOARD_STARTING_Y }, true);
	}
	gotoxy(startingPos, BOARD_STARTING_Y + 1);

	int whatSize = maxVisibleSize > sideLength ? sideLength : maxVisibleSize;

	for (int i = yOffset; i < whatSize + yOffset; i++) {
		for (int j = xOffset; j < whatSize + xOffset + 2; j++) {
			if (j == 0 || j == sideLength + 1) putch(186);
			else {
				if (status[i][j - 1] == 'w') {
					textcolor(WHITE);
					textbackground(WHITE);
				}
				else if (status[i][j - 1] == 'b') {
					textcolor(BLACK);
					textbackground(BLACK);
				}
				putch(197);
			}
			textcolor(DEFAULT_TEXT_COLOR);
			textbackground(BLACK);
			if (j != sideLength + 1) putch(196);
		}
		gotoxy(startingPos, wherey() + 1);
	}

	if (whatSize + yOffset == sideLength ||
		whatSize == sideLength) {
		m_drawHorizontalLine({ startingPos, wherey() }, false);
	}

	gotoxy(1, 1);
}

void Board::m_drawHorizontalLine(Pair pos, bool up)
{
	int whatSize = maxVisibleSize > sideLength ? sideLength : maxVisibleSize;
	gotoxy(pos.x, pos.y);
	for (int i = 0; i < whatSize + 2; i++) {
		if (i == 0) {
			putch(up ? 201 : 200); 
		}
		else if (i == whatSize + 1) {
			putch(205);
			putch(up ? 187 : 188);
		}
		else {
			putch(205);
			putch(205);
		}
	}
}

void Board::m_setBoardSize()
{
	textcolor(DEFAULT_TEXT_COLOR);
	cputs("Choose board size:\n");
	cputs("1. 9x9\n");
	cputs("2. 13x13\n");
	cputs("3. 19x19\n");
	cputs("4. Custom size\n");

	int chCode = getch();
	switch (chCode)
	{
	case 0x31:
		sideLength = 9;
		break;
	case 0x32:
		sideLength = 13;
		break;
	case 0x33:
		sideLength = 19;
		break;
	case 0x34:
		sideLength = m_setCustomBoardSize();
		break;
	default:
		m_invalidBoardSize();
		break;
	}
}

int Board::m_setCustomBoardSize()
{
	clrscr();
	gotoxy(1, 1);
	cputs("Choose custom board size:\n");

	char tmp[32] = { 0 };
	readString(tmp);

	return atoi(tmp);
}

void Board::m_invalidBoardSize()
{
	clrscr();
	gotoxy(1, 1);
	textcolor(4);
	cputs("Invalid input\n");
	textcolor(DEFAULT_TEXT_COLOR);
	m_setBoardSize();
}