#pragma once

#include "Base.h"
#include "Entity.h"
#include <iostream>
using namespace std;
using namespace sfw;
using namespace base;

class Enemy
{

public:
	char name[32] = "dinosaur";
	int hp = 50, hpMax = 50;
	int mp = 50, mpMax = 50;
	int atk = 15, atkMax = 15;
	int def = 15, defMax = 15;
	float spd = 30, spdMax = 30;
	int timer = 0;


};