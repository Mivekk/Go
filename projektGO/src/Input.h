#pragma once
#include "Cursor.h"
#include "Graph.h"
#include <string.h>

enum ARROWS {
	ARROW_UP = 0x48,
	ARROW_RIGHT = 0x4D,
	ARROW_DOWN = 0x50,
	ARROW_LEFT = 0x4B,
	ENTER = 0x0D,
	BACKSPACE = 0x08,
	ESC = 0x1B
};

class Input
{
private:
	bool* m_turn;
	bool* m_withHandicap;
	float* m_scoreBlack;
	float* m_scoreWhite;
	Board* m_board;
	Graph** m_graph;

private:
	bool m_confirmAction();
	void m_tileOccupied();
	char m_placeTile(Cursor& cursor);
	void m_handleGraph(Pair curPos);
	void m_saveGame();
	void m_loadGame();
	void m_loadGameStatus(int boardSize, char**& status, int newBoardSize, FILE*& file);
	void m_rememberLastPosition();
	void m_countPoints();
	bool m_isKO(Pair curPos);

public:
	Input();

	char handleInput(Board& board, Graph*& graph, Cursor& cursor, 
		bool& turn, bool& withHandicap, float& scoreBlack, float& scoreWhite);
};

int readString(char* finalStr);

template <typename T>
char* convertToString(T number, const char* mode) {
	char tmp[32] = { 0 };
	sprintf_s(tmp, mode, number);

	return tmp;
}