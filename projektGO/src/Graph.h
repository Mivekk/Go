#pragma once
#include "Board.h"
#include <stdlib.h>
#include <string.h>
#include "conio2.h"
#include <stdio.h> 

class Pair;

class Graph
{
private:
	int m_boardSize;
	bool* m_visited;
	Board* m_board;

private:
	void m_removeEdgeHelper(int from, int to);
	void m_DFS(int node, bool& didFind);
	void m_DFSEdgesRemover(int node, int prevNode, int& edgesRemoved);
	void m_touchesTilesDFS(int node, char& result, bool& foundAlready);
	void m_setFinalTilesDFS(int node, char result, int& territoryNum);

public:
	int** adj;
	int* adjSizes;
	bool* allEVisited;

	static int sBoardSideLength;

public:
	Graph();
	Graph(Board& board);
	~Graph();

	void addEdge(Pair from, Pair to);
	void removeEdge(Pair from, Pair to);
	bool hasLiberties(Pair node);
	int touchesTiles(Pair node);
	int removeEdgesDFS(Pair node);
};

