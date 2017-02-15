#pragma once

#include "Base.h"
#include "Entity.h"
#include <iostream>
using namespace std;
using namespace sfw;

class PlayerController
{

public:
	float speed = 100, turnSpeed = 10;

	float shotTimer = 0.0f;
	bool shotRequest = false;

	void poll(base::Transform *T, base::Rigidbody *rb, float dt)
	{
		if (sfw::getKey('W'))
			rb->addForce(T->getGlobalUp() * speed);

		if (sfw::getKey('A'))
			rb->addTorque(turnSpeed);

		if (sfw::getKey('D'))
			rb->addTorque(-turnSpeed);



		shotTimer -= dt;
		if (sfw::getKey(' ') && shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = 0.86f;
		}
		else shotRequest = false;

	}

};