#include "Pair.h"
#include "Graph.h"

Pair::Pair()
	: x(-1), y(-1)
{
}

Pair::Pair(int a, int b)
	: x(a), y(b)
{
}

int Pair::toInt()
{
	return this->x * Graph::sBoardSideLength + this->y;
}

int toIntG(Pair pair)
{
	// convert to b status dims
	return pair.x * Graph::sBoardSideLength + pair.y;
}

Pair toPair(int idx)
{
	// convert to b status dims
	return { idx / Graph::sBoardSideLength, (idx % Graph::sBoardSideLength) };
}