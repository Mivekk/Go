#pragma once
#include "Pair.h"
#include "constants.h"
#include "conio2.h"

class Cursor
{
public:
	Pair windowPos;
	Pair realPos;
	int color;

public:
	Cursor();
};

