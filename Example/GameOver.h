#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"
#include "Input.h"
#include <iostream>
using namespace std;
using namespace kpc;

/*
The gamestate represents a discrete container of all that is
necessary to drive the game experience.

A factory object is used to manage the creation and deletion of
objects in the simulation.

The game state updates the entities within the factory using
a series of 'systems.'
*/

class GameOver : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_ship, spr_bullet, spr_roid, spr_font, spr_bar;
	Player player;
	ObjectPool<Entity>::iterator currentCamera;
	bool paused = false;
	bool retry = false;
	bool quit = false;
public:

	virtual void init(Player p)
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet.png");
		spr_space = sfw::loadTextureMap("../res/space.jpg");
		spr_ship = sfw::loadTextureMap("../res/ship.png");
		spr_roid = sfw::loadTextureMap("../res/rock.png");
		spr_font = sfw::loadTextureMap("../res/font.png", 32, 4);
		spr_bar = sfw::loadTextureMap("../res/sidebar.png");
		player = p;
	}

	virtual void play()
	{

		// delete any old entities sitting around
		for (auto it = factory.begin(); it != factory.end(); it->onFree(), it.free());

		// setup a default camera
		currentCamera = factory.spawnCamera(800, 600, 1, vec2{ 0,0 });
		currentCamera->transform->setGlobalPosition(vec2{ 400, 300 });

		// call some spawning functions!
		factory.spawnStaticImage(spr_space, 0, 0, 800, 600);



	}

	virtual void stop()
	{

	}

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const
	{
		if (retry)
			return 0;

		else if (quit)
			return 9;

		else
			return 8;
	}


	// update loop, where 'systems' exist
	virtual void step()
	{
		if (!paused)
		{
			float dt = sfw::getDeltaTime();

			// maybe spawn some asteroids here.

			if (getKeyDown('Q'))
			{
				quit = true;
			}
			if (getKeyDown('R'))
			{
				retry = true;
			}


			// Physics system!
			// You'll want to extend this with custom collision responses


			
			//p
		}
	}


	virtual void draw()
	{
		// kind of round about, but this is the camera matrix from the factory's current camera
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		drawTexture(spr_bar, 75, 300, 150, 600, 0, true, 0, BLACK);

	}
};