#pragma once
#include "Board.h"
#include "conio2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Input.h"
#include "Graph.h"

class Game
{
private:
	bool m_withHandicap;
	bool m_turn;
	int m_controlsPosX;
	int m_screenWidth;
	int m_screenHeight;
	float m_scoreWhite;
	float m_scoreBlack;
	text_info m_info;
	Board m_board;
	Cursor m_cursor;
	Input m_input;
	Graph* m_graph;

private:
	void m_initGame();
	void m_startGame();
	void m_newGame();
	void m_displayCaption();
	void m_playWithHandicap();

public:
	Game();
	~Game();
};

