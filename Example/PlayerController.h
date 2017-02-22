#pragma once

#include "Base.h"
#include "Entity.h"
#include "Player.h"
#include <iostream>
using namespace std;
using namespace sfw;
using namespace base;

class PlayerController
{

public:
	float speed = 100;
	
	Player player;
	vec2 move;

	void poll(base::Transform *T, base::Rigidbody *rb, float dt)
	{
		move = vec2{ 0,0 };
		if (sfw::getKey('W'))
			move.y += 3000;

		else if (sfw::getKey('S'))
			move.y -= 3000;

		if (sfw::getKey('A'))
			move.x -= 3000;

		else if (sfw::getKey('D'))
			move.x += 3000;

		rb->addForce(move);

	}

};