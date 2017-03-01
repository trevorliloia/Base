#pragma once

#include "Base.h"
#include "Entity.h"
#include <iostream>
#include <time.h>
using namespace std;
using namespace sfw;
using namespace base;

class Enemy
{

public:
	char name[32] = "Hollow";
	int tmpHP = rand() % ((55 + 1) - 40) + 40;
	int hp = tmpHP, hpMax = tmpHP;
	int mp = 50, mpMax = 50;
	int atk = 10, atkMax = 10;
	int def = 15, defMax = 15;
	int tmpSPD = rand() % ((15 + 1) - 5) + 5;
	float spd = tmpSPD, spdMax = tmpSPD;
	float timer = 0;


};