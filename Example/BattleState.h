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

class BattleState : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_ship, spr_bullet, spr_roid, spr_font, spr_bar;
	Player player;
	ObjectPool<Entity>::iterator currentCamera;
	Transform menuP;
	UIText menuI[4];
	Menu baseM;
public:
	bool paused = false;
	bool done = false;
	bool turn = true;
	int timers[16];
	

	virtual void init(Player p)
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet.png");
		spr_space = sfw::loadTextureMap("../res/space.jpg");
		spr_ship = sfw::loadTextureMap("../res/ship.png");
		spr_roid = sfw::loadTextureMap("../res/rock.png");
		spr_font = sfw::loadTextureMap("../res/font.png", 32, 4);
		spr_bar = sfw::loadTextureMap("../res/sidebar.png");
		player = p;
		menuI[0].setString("Attack");
		menuI[1].setString("Tech");
		menuI[2].setString("Items");
		menuI[3].setString("Tactics");
		menuP.setGlobalPosition(vec2{ 20,500 });
		baseM = Menu(menuP, 4, menuI, true, true, spr_font);
	}

	virtual void play()
	{
		// delete any old entities sitting around
		for (auto it = factory.begin(); it != factory.end(); it->onFree(), it.free());

		// setup a default camera
		currentCamera = factory.spawnCamera(800, 600, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 400, 300 });

		// call some spawning functions!
		factory.spawnStaticImage(spr_space, 0, 0, 800, 600);

		factory.spawnPlayer(spr_ship, spr_font, player);
		for (int i = 0; i < player.enemyCount; i++)
		{
			factory.spawnEnemy(spr_roid, spr_font);
		}
		

	}

	virtual void stop()
	{

	}

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const
	{
		if (done)
			return 1;

		else
			return 3;
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
						player.timer += 1+(getDeltaTime() * player.spd);
					}
					else if (player.timer > 100)
					{
						player.timer = 100;
					}
					currentCamera->transform->setGlobalPosition(e.transform->getGlobalPosition() + 300);
					char bufferP[80];

					sprintf_s(bufferP, "%s\n%d", player.name, player.hp);
					e.text->setString(bufferP);
				}
				if (e.enemy)
				{
					if (e.enemy->timer < 100)
					{
						e.enemy->timer += 1+(getDeltaTime() * e.enemy->spd);
					}
					else if (e.enemy->timer > 100)
					{
						e.enemy->timer = 100;
					}
					timers[d] = e.enemy->timer;


					if (e.text)
					{
						char buffer[80];

						sprintf_s(buffer, "%s\n%d", e.enemy->name, e.enemy->hp);
						e.text->setString(buffer);
					}
					d++;
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

		for (int i = 0; i < player.enemyCount; i++)
		{
			for (int j = 0; j <= timers[i]; j++)
			{
				drawLine(25 + j, 55 + (i * 25), 25 + j, 75 + (i * 25), YELLOW);
			}
			drawString(spr_font, "time ", 25, 75 + (i * 25), 25, 25, 0, '\0', WHITE);
			cout << timers[i];
		}

		for (int k = 0; k <= player.timer; k++)
		{
			drawLine(25 + k, 25, 25 + k, 45, WHITE);
		}

		baseM.draw();

#ifdef _DEBUG
		for each(auto &e in factory)
			if (e.transform)
				e.transform->draw(cam);

		for each(auto &e in factory)
			if (e.transform && e.collider)
				e.collider->draw(&e.transform, cam);

		for each(auto &e in factory)
			if (e.transform && e.rigidbody)
				e.rigidbody->draw(&e.transform, cam);
#endif
	}
};