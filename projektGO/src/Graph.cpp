#include "Graph.h"
#include "Pair.h"

Graph::Graph()
	:adj(nullptr), adjSizes(nullptr), allEVisited(nullptr),
	m_visited(nullptr), m_boardSize(0),
	m_board(nullptr)
{
}

Graph::Graph(Board& board)
	: allEVisited(new bool[board.sideLength * board.sideLength]), 
	adj(new int* [board.sideLength * board.sideLength]),
	adjSizes(new int[board.sideLength * board.sideLength]),
	m_visited(new bool[board.sideLength * board.sideLength]),
	m_board(&board)
{	
	// static board side
	sBoardSideLength = board.sideLength;
	// full two dimensional board size
	m_boardSize = board.sideLength * board.sideLength;

	for (int i = 0; i < m_boardSize; i++) {
		// initialize values
		adj[i] = new int[4];
		adjSizes[i] = 0;
		m_visited[i] = false;
		allEVisited[i] = false;
	}
}

Graph::~Graph() {
	// clear up
	for (int i = 0; i < m_boardSize; i++) {
		delete[] adj[i];
	}
	delete[] adj;
	delete[] adjSizes;
	delete[] allEVisited;
	delete[] m_visited;
}

void Graph::addEdge(Pair from, Pair to)
{
	adj[from.toInt()][adjSizes[from.toInt()]] = to.toInt();
	adjSizes[from.toInt()]++;

	adj[to.toInt()][adjSizes[to.toInt()]] = from.toInt();
	adjSizes[to.toInt()]++;
}

void Graph::removeEdge(Pair from, Pair to)
{
	// remove edge from a to b
	m_removeEdgeHelper(from.toInt(), to.toInt());

	// remove edge from b to a
	m_removeEdgeHelper(to.toInt(), from.toInt());
}

int Graph::touchesTiles(Pair node)
{
	memset(m_visited, false, m_boardSize);

	bool foundAlready = false;
	char result = 'e';
	m_touchesTilesDFS(node.toInt(), result, foundAlready);

	int territoryNum = 0;

	memset(m_visited, false, m_boardSize);
	m_setFinalTilesDFS(node.toInt(), result, territoryNum);

	return territoryNum;
}

void Graph::m_touchesTilesDFS(int node, char& result, bool& foundAlready)
{
	m_visited[node] = true;
	allEVisited[node] = true;

	int curY = toPair(node).x;
	int curX = toPair(node).y;

	struct {
		int a, b;
	} adjIndexes[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

	for (auto ics : adjIndexes) {
		if (curY + ics.b < m_board->sideLength &&
			curY + ics.b >= 0 &&
			curX + ics.a < m_board->sideLength &&
			curX + ics.a >= 0)
		{																	   
			if (m_board->status[curY + ics.b][curX + ics.a] == 'b') {
				if (!foundAlready) {
					result = 'b';
					foundAlready = true;
				}
				else if (result == 'w') {
					result = 'e';
				}
			}
			else if (m_board->status[curY + ics.b][curX + ics.a] == 'w') {
				if (!foundAlready) {
					result = 'w';
					foundAlready = true;
				}
				else if (result == 'b') {
					result = 'e';
				}
			}
		}																	   
	}

	for (int i = 0; i < adjSizes[node]; i++) {
		if (!m_visited[adj[node][i]]) {
			m_touchesTilesDFS(adj[node][i], result, foundAlready);
		}
	}
}

void Graph::m_setFinalTilesDFS(int node, char result, int& territoryNum)
{
	m_visited[node] = true;

	m_board->finalStatus[toPair(node).x][toPair(node).y] = result;
	if (!m_visited[node]) {
		territoryNum++;
	}

	for (int i = 0; i < adjSizes[node]; i++) {
		if (!m_visited[adj[node][i]]) {
			m_setFinalTilesDFS(adj[node][i], result, territoryNum);
		}
	}
}

void Graph::m_removeEdgeHelper(int a, int b) {
	int curAdjSize = adjSizes[a];
	for (int i = 0; i < curAdjSize; i++) {
		if (adj[a][i] == b) {
			// move last adj element to this pos 
			// because order of adj elements does not matter
			if (i != curAdjSize - 1) {
				adj[a][i] = adj[a][curAdjSize - 1];
			}

			adjSizes[a]--;
			break;
		}
	}
}

bool Graph::hasLiberties(Pair node)
{
	memset(m_visited, false, m_boardSize);

	bool didFind = false;

	m_DFS(node.toInt(), didFind);
	return didFind;
}

void Graph::m_DFS(int node, bool& didFind)
{
	m_visited[node] = true;

	struct {
		int a, b;
	} adjIndexes[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

	int curX = toPair(node).y;
	int curY = toPair(node).x;

	for (auto ics : adjIndexes) {
		if (curY + ics.b < m_board->sideLength &&
			curY + ics.b >= 0 &&
			curX + ics.a < m_board->sideLength &&
			curX + ics.a >= 0)
		{
			if (m_board->status[curY + ics.b][curX + ics.a] == 'e') {
				didFind = true;
				return;
			}
		}
	}

	for (int i = 0; i < adjSizes[node]; i++) {
		if (!m_visited[adj[node][i]]) {
			m_DFS(adj[node][i], didFind);
		}
	}
}

// returns how many edges removed
int Graph::removeEdgesDFS(Pair node)
{
	memset(m_visited, false, m_boardSize);

	int edgesRemoved = 0;
	m_DFSEdgesRemover(node.toInt(), node.toInt(), edgesRemoved);
	
	//m_board.drawBoard();

	return edgesRemoved;
}

void Graph::m_DFSEdgesRemover(int node, int prevNode, int& edgesRemoved)
{
	if (!m_visited[node]) {
		edgesRemoved++;
	}
	m_visited[node] = true;

	m_board->status[toPair(node).x][toPair(node).y] = 'e';

	while (adjSizes[node]) {
		if (node != prevNode) {
			removeEdge(toPair(node), toPair(prevNode));
		}
		m_DFSEdgesRemover(adj[node][0], node, edgesRemoved);
	}
}

int Graph::sBoardSideLength = 0;