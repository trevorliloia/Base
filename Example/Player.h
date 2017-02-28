#pragma once

#include "Base.h"
#include "Entity.h"
#include <iostream>

class Player
{
	
public:
	int invSize;

	char name[32] = "Crono";
	int hp = 100, hpMax = 100;
	int mp = 100, mpMax = 100;
	int atk = 35, atkMax = 35;
	int def = 35, defMax = 35;
	int spd = 35, spdMax = 35;
	int timer = 0;
	int heat = 0;

	int enemyCount = 3;

};