#pragma once

#include "Base.h"
#include "Entity.h"
#include <iostream>

class Player
{
	
public:
	int invSize;

	char name[32] = "Tarkus";
	int hp = 100, hpMax = 100;
	int mp = 100, mpMax = 100;
	int mag = 35, magMax = 35;
	int atk = 35, atkMax = 35;
	bool counter = false;
	int def = 35, defMax = 35;
	bool defending = false;
	float spd = 35, spdMax = 35;
	float timer = 0;
	int heat = 0;

	int enemyCount = 0;
	bool removeEnemy = false;

};