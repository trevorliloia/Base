#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"
#include "Menu.h"
#include "UIText.h"
#include <iostream>
using namespace std;
using namespace sfw;

/*
The gamestate represents a discrete container of all that is
necessary to drive the game experience.

A factory object is used to manage the creation and deletion of
objects in the simulation.

The game state updates the entities within the factory using
a series of 'systems.'
*/

class MenuState : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_ship, spr_bullet, spr_roid, spr_font, spr_bar;
	Player player;
	ObjectPool<Entity>::iterator currentCamera;
	Transform menuP;
	Transform EnemyT;
	UIText menuI[2];
	Menu baseM;


public:
	bool paused = false;
	bool start = false;
	bool quit = false;
	bool gameOver = false;
	bool turn = true;
	bool pickE = false;
	Menu sourceM;
	int tmpID;
	int timers[16];


	virtual void init(Player p)
	{


		spr_bullet = sfw::loadTextureMap("../res/bullet.png");
		spr_space = sfw::loadTextureMap("../res/space2.jpg");
		spr_ship = sfw::loadTextureMap("../res/TARKUS.png");
		spr_roid = sfw::loadTextureMap("../res/hollow2.png");
		spr_font = sfw::loadTextureMap("../res/font.png", 32, 4);
		spr_bar = sfw::loadTextureMap("../res/sidebar.png");
		player = p;

		menuI[0].setString("Start");
		menuI[1].setString("Quit");
		menuP.setGlobalPosition(vec2{ 10,550 });
		baseM = Menu(menuP, 2, menuI, true, true, spr_font);

		
	}

	virtual void play()
	{
		// delete any old entities sitting around
		for (auto it = factory.begin(); it != factory.end(); it->onFree(), it.free());

		// setup a default camera
		currentCamera = factory.spawnCamera(500, 500, 1, vec2{ 0,0 });
		currentCamera->transform->setGlobalPosition(vec2{ 400, 300 });

		// call some spawning functions!
		factory.spawnStaticImage(spr_space, 0, 0, 1200, 800);

		factory.spawnPlayer(spr_ship, spr_font, player, vec2{ 0, 0 });
		


		srand(time(NULL));
		start = false;
		quit = false;
	}

	virtual void stop()
	{

	}

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const
	{
		if (start)
			return 0;

		if (quit)
			return 9;

		else
			return 11;
	}


	// update loop, where 'systems' exist
	virtual void step()
	{

		if (!paused)
		{

			float dt = sfw::getDeltaTime();

			// maybe spawn some asteroids here.
			int d = 0;
			for (auto it = factory.begin(); it != factory.end();) // no++!
			{
				bool del = false; // does this entity end up dying?
				auto &e = *it;    // convenience reference

								  // rigidbody update

								  // controller update
				if (e.transform && e.rigidbody && e.controller)
				{
					if (player.timer < 100)
					{
						player.timer += (getDeltaTime() * (player.spd / 2));
					}
					else if (player.timer > 100)
					{
						player.timer = 100;
					}
					currentCamera->transform->setGlobalPosition(e.transform->getGlobalPosition() + 300);
					char bufferP[80];

					sprintf_s(bufferP, "%s\n%s", "B.I.T. Battle", "(Black Iron Tarkus)");
					e.text->setString(bufferP);
				}
				
				// lifetime decay update
				if (e.lifetime)
				{
					e.lifetime->age(dt);
					if (!e.lifetime->isAlive())
						del = true;
				}

				// ++ here, because free increments in case of deletions
				if (!del) it++;
				else
				{
					it->onFree();
					it.free();
				}


			}

			baseM.navigate();

			if (baseM.menuitems[0].pick)
			{
				baseM.menuitems[0].pick = false;
				start = true;
			}

			if (baseM.menuitems[1].pick)
			{
				baseM.menuitems[1].pick = false;
				quit = true;
			}

		


		}
	}




	virtual void draw()
	{
		// kind of round about, but this is the camera matrix from the factory's current camera
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		// draw sprites
		for each(auto &e in factory)
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);

		// draw text
		for each(auto &e in factory)
			if (e.transform && e.text)
				e.text->draw(&e.transform, cam);

		drawTexture(spr_bar, 75, 300, 150, 600, 0, true, 0, CYAN);


		


		baseM.draw();

	}
};