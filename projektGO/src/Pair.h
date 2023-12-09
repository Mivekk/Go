#pragma once

class Pair
{
public:
	int x, y;

public:
	Pair();
	Pair(int a, int b);

	int toInt();
};

int toIntG(Pair pair);
Pair toPair(int idx);