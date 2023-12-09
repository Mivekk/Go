#define _CRT_SECURE_NO_WARNINGS
#include "Input.h"

#define ITERATE_ADJ(macro) 				\
for (auto ics : adjIndexes) {											   \
	if (curY + ics.b < m_board->sideLength &&							   \
		curY + ics.b >= 0 &&											   \
		curX + ics.a < m_board->sideLength &&							   \
		curX + ics.a >= 0)												   \
	{																	   \
		macro															   \
	}																	   \
}	\

Input::Input()
	: m_turn(nullptr), m_withHandicap(nullptr), m_scoreBlack(nullptr), 
	m_scoreWhite(nullptr), m_board(nullptr), m_graph(nullptr)
{
}


char Input::handleInput(Board& board, Graph*& graph, Cursor& cursor,
	bool& turn, bool& withHandicap, float& scoreBlack, float& scoreWhite)
{
	m_turn = &turn;
	m_withHandicap = &withHandicap;
	m_scoreBlack = &scoreBlack;
	m_scoreWhite = &scoreWhite;
	m_board = &board;
	m_graph = &graph;

	int input = getch();
	if (input == ARROW_UP && cursor.realPos.y > 1 + BOARD_STARTING_Y) {
		// go down
		m_board->clearLastPosition(cursor);
		cursor.realPos.y--;

		if (cursor.windowPos.y < BOARD_STARTING_Y + 2) {
			m_board->yOffset--;
			clrscr();
			m_board->drawBoard();
		}
		else {
			cursor.windowPos.y--;
		}
	}
	else if (input == ARROW_DOWN && cursor.realPos.y < m_board->sideLength + BOARD_STARTING_Y) {
		// go up
		m_board->clearLastPosition(cursor);
		cursor.realPos.y++;

		if (cursor.windowPos.y >= m_board->maxVisibleSize + BOARD_STARTING_Y) {
			m_board->yOffset++;
			clrscr();
			m_board->drawBoard();
		}
		else {
			cursor.windowPos.y++;
		}
	}
	else if (input == ARROW_LEFT && cursor.realPos.x > 2 +
		(BOARD_DISPLAY_SIDE == "RIGHT" ? BOARD_STARTING_RIGHT_X : 3)) {
		// go left
		m_board->clearLastPosition(cursor);
		cursor.realPos.x -= 2;

		if (cursor.windowPos.x <= DETERMINE_BOARD + 2) {
			m_board->xOffset -= 1;
			clrscr();
			m_board->drawBoard();
		}
		else {
			cursor.windowPos.x -= 2;
		}
	}
	else if (input == ARROW_RIGHT && cursor.realPos.x < m_board->sideLength * 2 +
		(BOARD_DISPLAY_SIDE == "RIGHT" ? BOARD_STARTING_RIGHT_X - 1 : 2)) {
		// go right
		m_board->clearLastPosition(cursor);	
		cursor.realPos.x += 2;

		if (cursor.windowPos.x >= m_board->maxVisibleSize * 2 + DETERMINE_BOARD) {
			m_board->xOffset += 1;
			clrscr();
			m_board->drawBoard();
		}
		else {
			cursor.windowPos.x += 2;
		}

	}
	else if (input == 'n') {
		// new game
		if (m_confirmAction()) {
			return 'n';
		}
	}
	else if (input == 'i') {
		// place tile
		m_placeTile(cursor);
	}
	else if (input == 's') {
		m_saveGame();
	}
	else if (input == 'l') {
		m_loadGame();
	}
	else if (input == ESC && *m_withHandicap) {
		if (m_confirmAction()) {
			*m_withHandicap = false;
			*m_turn = WHITE_TURN;
		}
	}
	else if (input == 'f') {
		//m_countPoints();
		//board.drawBoard();
	}
	else if (input == 'q') {
		// quit
		return 'q';
	}

	// is game alive = yes
	return 'y';
}

// finalStr must be large enough
// returns size of string
int readString(char* finalStr)
{
	int chCode = -1;
	do {
		chCode = getch();
		if (chCode == BACKSPACE) {
			cputs("\b \b");
			if (strlen(finalStr) > 0) {
				finalStr[strlen(finalStr) - 1] = '\0';
			}
		}
		else {
			cputs(convertToString(chCode, "%c"));
			finalStr[strlen(finalStr)] = (char)chCode;
		}
	} while (chCode != ENTER);


	finalStr[strlen(finalStr) - 1] = '\0';
	return (int)strlen(finalStr);
}

void Input::m_tileOccupied()
{
	gotoxy(DETERMINE_BOARD, 1);
	textcolor(RED);
	textbackground(BLACK);
	const char* confirmText = "Tile is already occupied";
	cputs(confirmText);
}

bool Input::m_confirmAction()
{
	bool result = false;
	gotoxy(DETERMINE_BOARD, 1);
	textcolor(LIGHTBLUE);
	textbackground(BLACK);
	const char* confirmText = "Press ENTER to confirm or ESC to cancel";
	cputs(confirmText);

	int input = 0;
	while (input != ESC && input != ENTER) {
		input = getch();
		if (input == ESC) {
			result = false;
		}
		else if (input == ENTER) {
			result = true;
		}
	}

	gotoxy(wherex() + 1, wherey());
	for (int i = 0; i < strlen(confirmText); i++) {
		gotoxy(wherex() - 1, wherey());
		cputs("\b ");
	}

	return result;
}

char Input::m_placeTile(Cursor& cursor)
{
	// test if tile is already occupied
	if (m_board->status[cursor.realPos.y - BOARD_STARTING_Y - 1]
		[(cursor.realPos.x - DETERMINE_BOARD) / 2 - 1] != 'e') {
		m_tileOccupied();
		// is game alive yes
		return 'y';
	}

	// when i pressed, set current char to turn color
	gotoxy(wherex() - 1, wherey());
	textcolor(*m_turn ? WHITE : BLACK);
	textbackground(*m_turn ? WHITE : BLACK);
	// put char with textcolor and bgcolor as above
	putch('*');

	// NODES CONVERT TO B STATUS
	// NODES  B STATUS  X AND Y
	// 0 1 2  00 01 02  00 10 20    
	// 3 4 5  10 11 12  01 11 21
	// 6 7 8  20 21 22  02 12 22

	int curX = (cursor.realPos.x - DETERMINE_BOARD) / 2 - 1;
	int curY = cursor.realPos.y - BOARD_STARTING_Y - 1;
	if (*m_turn == BLACK_TURN && m_confirmAction()) {
		// set board status
		// inverted x and y
		m_board->status[curY][curX] = 'b';
		cursor.color = LIGHTBLUE;
		if (!(*m_withHandicap)) {
			*m_turn = WHITE_TURN;
		}
		
		m_handleGraph({ curX, curY });
		m_board->drawBoard();

		m_rememberLastPosition();
	}
	else if (*m_turn == WHITE_TURN && m_confirmAction()) {
		// set board status
		// inverted x and y
		m_board->status[curY][curX] = 'w';
		cursor.color = LIGHTBLUE;
		*m_turn = BLACK_TURN;
		m_handleGraph({ curX, curY });
		m_board->drawBoard();

		m_rememberLastPosition();
	}
	return 'y';
}

void Input::m_handleGraph(Pair curPos)
{
	int curX = curPos.x;
	int curY = curPos.y;

	// add edge between itself and same color adjacent nodes
	// perform dfs on four adjacent nodes and itself
	struct {
		int a, b;
	} adjIndexes[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

	ITERATE_ADJ(
		if (m_board->status[ics.b + curY][ics.a + curX] ==
			m_board->status[curY][curX]) {
			(*m_graph)->addEdge({ curY, curX }, { curY + ics.b, curX + ics.a });
		}
	);

	// if borders opposite color that dont have liberties
	if (!(*m_graph)->hasLiberties({ curY, curX })) {
		bool bordersOp = false;
		ITERATE_ADJ(
			if (m_board->status[ics.b + curY][ics.a + curX] !=
				m_board->status[curY][curX] &&
				m_board->status[ics.b + curY][ics.a + curX] != 'e' &&
				!(*m_graph)->hasLiberties({ curY + ics.b, curX + ics.a })) {
				bordersOp = true;
			}
		)

		if (!bordersOp) {
			// delete all edges
			ITERATE_ADJ(
				if (m_board->status[ics.b + curY][ics.a + curX] ==
					m_board->status[curY][curX]) {
					(*m_graph)->removeEdge({ curY, curX }, { curY + ics.b, curX + ics.a });
				}
			)

			m_board->status[curY][curX] = 'e';
			*m_turn = !(*m_turn);
		}
	}

	// check every adj node if has liberties
	ITERATE_ADJ(
		if (m_board->status[ics.b + curY][ics.a + curX] != 'e' &&
			!(*m_graph)->hasLiberties({ curY + ics.b, curX + ics.a })) {
			char nodeRemoved = m_board->status[curY + ics.b][curX + ics.a];
			int points = (*m_graph)->removeEdgesDFS({ curY + ics.b, curX + ics.a });
	
			if (points == 1 && m_isKO({ curX, curY })) {
				m_board->status[curY + ics.b][curX + ics.a] = nodeRemoved;
				return;
			}

			if (m_board->status[curY][curX] == 'w') {
				*m_scoreWhite += points;
			}
			else {
				*m_scoreBlack += points;
			}
		}
	)
}

void Input::m_saveGame()
{
	gotoxy(DETERMINE_BOARD, 1);
	textcolor(LIGHTBLUE);
	textbackground(BLACK);
	const char* saveGameText = "Enter file name to save game: ";
	cputs(saveGameText);

	char tmpName[259] = { 0 };
	int fileNameLength = readString(tmpName);

	char* finalName = new char[fileNameLength + 1 + 4];
	strcpy(finalName, tmpName);
	finalName[fileNameLength] = '.';
	finalName[fileNameLength + 1] = 't';
	finalName[fileNameLength + 2] = 'x';
	finalName[fileNameLength + 3] = 't';
	finalName[fileNameLength + 4] = '\0';

	if (!m_confirmAction()) {
		delete[] finalName;
		return;
	}

	// write to file
	FILE* file = fopen(finalName, "w");
	// write boardStatus
	fwrite(&m_board->sideLength, sizeof(int), 1, file);
	for (int i = 0; i < m_board->sideLength; i++) {
		fwrite(m_board->status[i], 1, m_board->sideLength, file);
	}
	for (int i = 0; i < m_board->sideLength; i++) {
		fwrite(m_board->lastBlackStatus[i], 1, m_board->sideLength, file);
	}
	for (int i = 0; i < m_board->sideLength; i++) {
		fwrite(m_board->lastWhiteStatus[i], 1, m_board->sideLength, file);
	}

	fwrite(m_turn, 1, 1, file);
	fwrite(m_scoreBlack, sizeof(float), 1, file);
	fwrite(m_scoreWhite, sizeof(float), 1, file);

	// save graph
	for (int i = 0; i < m_board->sideLength * m_board->sideLength; i++) {
		fwrite((*m_graph)->adj[i], sizeof(int), 4, file);
	}
	fwrite((*m_graph)->adjSizes, sizeof(int), m_board->sideLength * m_board->sideLength, file);

	fclose(file);

	gotoxy(wherex() + 1, wherey());
	for (int i = 0; i < strlen(saveGameText) + fileNameLength; i++) {
		gotoxy(wherex() - 1, wherey());
		cputs("\b ");
	}
	cputs(finalName);

	delete[] finalName;
}

void Input::m_loadGame()
{
	gotoxy(DETERMINE_BOARD, 1);
	textcolor(LIGHTBLUE);
	textbackground(BLACK);
	const char* saveGameText = "Enter file name to load game: ";
	cputs(saveGameText);

	char tmpName[259] = { 0 };
	int fileNameLength = readString(tmpName);

	char* finalName = new char[fileNameLength + 1 + 4];
	strcpy(finalName, tmpName);
	finalName[fileNameLength] = '.';
	finalName[fileNameLength + 1] = 't';
	finalName[fileNameLength + 2] = 'x';
	finalName[fileNameLength + 3] = 't';
	finalName[fileNameLength + 4] = '\0';

	if (!m_confirmAction()) {
		delete[] finalName;
		return;
	}

	FILE* file = fopen(finalName, "r");
	if (file == nullptr) {
		gotoxy(wherex() + 1, wherey());
		for (int i = 0; i < strlen(saveGameText) + fileNameLength; i++) {
			gotoxy(wherex() - 1, wherey());
			cputs("\b ");
		}
		textcolor(4);
		gotoxy(DETERMINE_BOARD, 1);
		cputs("Incorrect file name!");

		delete[] finalName;
		return;
	}

	int newBoardSize;
	fread(&newBoardSize, sizeof(int), 1, file);

	m_loadGameStatus(m_board->sideLength, m_board->status, newBoardSize, file);
	m_loadGameStatus(m_board->sideLength, m_board->lastBlackStatus, newBoardSize, file);
	m_loadGameStatus(m_board->sideLength, m_board->lastWhiteStatus, newBoardSize, file);

	m_board->sideLength = newBoardSize;
	Graph::sBoardSideLength = newBoardSize;
	bool bufferTurn;
	float bufferScoreBlack, bufferScoreWhite;
	fread(&bufferTurn, 1, 1, file);
	fread(&bufferScoreBlack, sizeof(float), 1, file);
	fread(&bufferScoreWhite, sizeof(float), 1, file);
	
	delete *m_graph;
	*m_graph = new Graph(*m_board);

	// load graph
	for (int i = 0; i < m_board->sideLength * m_board->sideLength; i++) {
		fread((*m_graph)->adj[i], sizeof(int), 4, file);
	}
	fread((*m_graph)->adjSizes, sizeof(int), m_board->sideLength * m_board->sideLength, file);

	*m_turn = bufferTurn;
	*m_scoreBlack = bufferScoreBlack;
	*m_scoreWhite = bufferScoreWhite;

	clrscr();
	m_board->drawBoard();

	fclose(file);

	delete[] finalName;
}

void Input::m_loadGameStatus(int boardSize, char**& status, int newBoardSize, FILE*& file)
{
	for (int i = 0; i < boardSize; i++) {
		delete[] status[i];
	}
	delete[] status;

	status = new char* [newBoardSize];
	for (int i = 0; i < newBoardSize; i++) {
		status[i] = new char[newBoardSize];
	}

	for (int i = 0; i < newBoardSize; i++) {
		fread(status[i], 1, newBoardSize, file);
	}
}

void Input::m_rememberLastPosition()
{
	// remember last board status when white had turn
	// remember last board status when black had turn
	for (int i = 0; i < m_board->sideLength; i++) {
		for (int j = 0; j < m_board->sideLength; j++) {
			// inverted colors because turn changes before the function
			if (*m_turn == WHITE_TURN) {
				m_board->lastBlackStatus[i][j] = m_board->status[i][j];
			}
			else {
				m_board->lastWhiteStatus[i][j] = m_board->status[i][j];
			}
		}
	}

}

void Input::m_countPoints()
{
	struct {
		int a, b;
	} adjIndexes[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

	// go through the whole array
	// add edges between e's
	for (int curY = 0; curY < m_board->sideLength; curY++) {
		for (int curX = 0; curX < m_board->sideLength; curX++) {
			ITERATE_ADJ(
				if (m_board->status[ics.b + curY][ics.a + curX] ==
					m_board->status[curY][curX] && m_board->status[curY][curX] == 'e') {
					(*m_graph)->addEdge({ curY, curX }, { curY + ics.b, curX + ics.a });
				}
			)
		}
	}

	// territory
	for (int i = 0; i < m_board->sideLength; i++) {
		for (int j = 0; j < m_board->sideLength; j++) {
			if (m_board->status[i][j] == 'e' && !(*m_graph)->allEVisited[toIntG({ i, j })]) {
				(*m_graph)->touchesTiles({ i, j });
			}
		}
	}

	for (int i = 0; i < m_board->sideLength; i++) {
		for (int j = 0; j < m_board->sideLength; j++) {
			m_board->status[i][j] = m_board->finalStatus[i][j];
			if (m_board->status[i][j] == 'w') {
				(*m_scoreWhite)++;
			}
			else if (m_board->status[i][j] == 'b') {
				(*m_scoreBlack)++;
			}
		}
	}
}

bool Input::m_isKO(Pair curPos)
{
	int curX = curPos.x;
	int curY = curPos.y;

	// if new board status matches last board status
	for (int i = 0; i < m_board->sideLength; i++) {
		for (int j = 0; j < m_board->sideLength; j++) {
			// inverted colors because turn changes before the function
			if (*m_turn == WHITE_TURN &&
					m_board->lastBlackStatus[i][j] != m_board->status[i][j]) {
				return false;
			}
			else if (*m_turn == BLACK_TURN &&
					m_board->lastWhiteStatus[i][j] != m_board->status[i][j]){
				return false;
			}
		}
	}

	//  go to last board status when opposite color had turn
	for (int i = 0; i < m_board->sideLength; i++) {
		for (int j = 0; j < m_board->sideLength; j++) {
			// inverted colors because turn changes before the function
			if (*m_turn == BLACK_TURN) {
				m_board->status[i][j] = m_board->lastWhiteStatus[i][j];
			}
			else {
				m_board->status[i][j] = m_board->lastBlackStatus[i][j];
			}
		}
	}
	m_board->status[curY][curX] = 'e';

	m_board->drawBoard();

	//  reverse score and turn
	*m_turn = !(*m_turn);

	return true;
}