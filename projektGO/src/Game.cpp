#include "Game.h"

Game::Game()
	: m_graph(nullptr),
	m_scoreWhite(6.5f), m_scoreBlack(0.0f), 
	m_screenWidth(0), m_screenHeight(0), m_turn(BLACK_TURN)
{
	m_initGame();
}

Game::~Game()
{
	delete m_graph;
}

void Game::m_initGame() {
	m_board.boardInit();
	m_graph = new Graph(m_board);

	gotoxy(1, 1);
	m_playWithHandicap();
	if (m_withHandicap) m_scoreWhite = 0.5f;
	
	gettextinfo(&m_info);
	int verticalMaxSize = m_info.screenheight - BOARD_STARTING_Y - 2;
	int horizontalMaxSize = (m_info.screenwidth - BOARD_STARTING_RIGHT_X) / 2 - 4;
	m_board.maxVisibleSize = verticalMaxSize < horizontalMaxSize ? verticalMaxSize : horizontalMaxSize;

	if (BOARD_DISPLAY_SIDE == "RIGHT") {
		m_controlsPosX = 1;
		m_cursor.windowPos = { BOARD_STARTING_RIGHT_X + 2, BOARD_STARTING_Y + 1 };
	}
	else {
		m_controlsPosX = m_info.screenwidth - BOARD_CONTROLS_LEFT_X_OFFSET;
		m_cursor.windowPos = { BOARD_STARTING_LEFT_X + 2, BOARD_STARTING_Y + 1 };
	}
	m_cursor.realPos = m_cursor.windowPos;

	clrscr();
	m_board.drawBoard();

	m_startGame();
}

void Game::m_startGame() {
	char isGameAlive = 'y';
	do {
		_setcursortype(_NOCURSOR);

		// draw caption 
		m_displayCaption();

		// draw the cursor
		textcolor(m_cursor.color);
		textbackground(m_cursor.color);
		gotoxy(m_cursor.windowPos.x, m_cursor.windowPos.y);
		putch('*');
		textbackground(BLACK);

		isGameAlive = m_input.handleInput(m_board, m_graph, m_cursor, m_turn, 
			m_withHandicap, m_scoreBlack, m_scoreWhite);

		textcolor(DEFAULT_TEXT_COLOR);
		textbackground(BLACK);
		
	} while (isGameAlive == 'y');

	if (isGameAlive == 'n') {
		m_newGame();
	}
}

void Game::m_newGame() {
	clrscr();
	gotoxy(1, 1);
	
	Game game;
}

void Game::m_displayCaption()
{
	// controls
	gotoxy(m_controlsPosX, 2);
	cputs("Controls: ");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("arrows = moving");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("q      = exit");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("n      = start a new game");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("enter  = confirm choice and end turn");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("esc    = cancel current action");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("i      = place a stone on the board");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("s      = save the game state");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("l      = load the game state");
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("f      = finish the game");


	gotoxy(m_controlsPosX, wherey() + 2);
	cputs("Turn: ");
	cputs(m_turn == BLACK_TURN ? "Black" : "White");

	// cursor position
	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("                            ");
	gotoxy(m_controlsPosX, wherey());
	cputs("Cursor position: [ ");

	cputs(convertToString((m_cursor.realPos.x - DETERMINE_BOARD) / 2, "%d"));
	cputs(", ");

	cputs(convertToString(m_cursor.realPos.y - BOARD_STARTING_Y, "%d"));
	cputs(" ]");
	
	// score
	gotoxy(m_controlsPosX, wherey() + 2);

	cputs("Score black: ");
	cputs(convertToString(m_scoreBlack, "%.1f"));

	gotoxy(m_controlsPosX, wherey() + 1);
	cputs("Score white: ");
	cputs(convertToString(m_scoreWhite, "%.1f"));

	gotoxy(m_controlsPosX, wherey() + 2);
	cputs("Lukasz Machutt 193517");

	gotoxy(1, 1);
}

void Game::m_playWithHandicap()
{
	clrscr();
	cputs("Play with handicap\n");
	cputs("1. Yes\n");
	cputs("2. No");

	do {
		int idx = getch();

		if (idx == '1') {
			m_withHandicap = true;
			return;
		}
		else if (idx == '2') {
			m_withHandicap = false;
			return;
		}
	} while (true);
}