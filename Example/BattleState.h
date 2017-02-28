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

	UIText attackText[3];
	int atkID[3];
	Menu attackM;

	UIText techText[3];
	int techID[3];
	Menu techM;

	UIText itemText[1];
	int itemID[1];
	Menu itemM;

	UIText tacText[4];
	int tacID[4];
	Menu tacM;
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

		attackText[0].setString("Slash");
		atkID[0] = 1;
		attackText[1].setString("Pierce");
		atkID[1] = 2;
		attackText[2].setString("Charge");
		atkID[2] = 3;
		attackM = Menu(menuP, 3, attackText, false, false, spr_font, atkID);

		techText[0].setString("Fire Rush");
		techID[0] = 4;
		techText[1].setString("Spark Cut");
		techID[1] = 5;
		techText[2].setString("Heat Burst");
		techID[2] = 6;
		techM = Menu(menuP, 3, techText, false, false, spr_font, techID);

		itemText[0].setString("No Items");
		itemM = Menu(menuP, 1, itemText, false, false, spr_font);

		tacText[0].setString("Defend");
		tacID[0] = 7;
		tacText[1].setString("Charge Heat");
		tacID[1] = 8;
		tacText[2].setString("Counter");
		tacID[2] = 9;
		tacText[3].setString("Run");
		tacID[3] = 10;
		tacM = Menu(menuP, 4, tacText, false, false, spr_font, tacID);


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
			attackM.navigate();
			techM.navigate();
			itemM.navigate();
			tacM.navigate();

			if (baseM.menuitems[0].pick)
			{
				baseM.menuitems[0].pick = false;
				attackM.isOpen = true;
				attackM.isVisible = true;
				baseM.isOpen = false;
				baseM.isVisible = false;
			}

			if (baseM.menuitems[1].pick)
			{
				baseM.menuitems[1].pick = false;
				techM.isOpen = true;
				techM.isVisible = true;
				baseM.isOpen = false;
				baseM.isVisible = false;
			}

			if (baseM.menuitems[2].pick)
			{
				baseM.menuitems[2].pick = false;
				itemM.isOpen = true;
				itemM.isVisible = true;
				baseM.isOpen = false;
				baseM.isVisible = false;
			}

			if (baseM.menuitems[3].pick)
			{
				baseM.menuitems[3].pick = false;
				tacM.isOpen = true;
				tacM.isVisible = true;
				baseM.isOpen = false;
				baseM.isVisible = false;
			}

			if (attackM.isOpen && kpc::getKeyDown('Q'))
			{
				attackM.isOpen = false;
				attackM.isVisible = false;
				baseM.isOpen = true;
				baseM.isVisible = true;
			}
			if (techM.isOpen && kpc::getKeyDown('Q'))
			{
				techM.isOpen = false;
				techM.isVisible = false;
				baseM.isOpen = true;
				baseM.isVisible = true;
			}
			if (itemM.isOpen && kpc::getKeyDown('Q'))
			{
				itemM.isOpen = false;
				itemM.isVisible = false;
				baseM.isOpen = true;
				baseM.isVisible = true;
			}
			if (tacM.isOpen && kpc::getKeyDown('Q'))
			{
				tacM.isOpen = false;
				tacM.isVisible = false;
				baseM.isOpen = true;
				baseM.isVisible = true;
			}

			for (int i = 0; i < attackM.maxItem; i++)
			{
				if (attackM.menuitems[i].pick)
				{
					switch (attackM.menuitems[i].useID)
					{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					default:
						break;
					}
				}
			}

			for (int i = 0; i < techM.maxItem; i++)
			{
				if (techM.menuitems[i].pick)
				{
					switch (techM.menuitems[i].useID)
					{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					default:
						break;
					}
				}
			}

			for (int i = 0; i < tacM.maxItem; i++)
			{
				if (tacM.menuitems[i].pick)
				{
					switch (tacM.menuitems[i].useID)
					{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					default:
						break;
					}
				}
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

		for (int i = 0; i < player.enemyCount; i++)
		{
			for (int j = 0; j <= timers[i]; j++)
			{
				drawLine(25 + j, 55 + (i * 25), 25 + j, 75 + (i * 25), YELLOW);
			}
			drawString(spr_font, "time ", 25, 75 + (i * 25), 25, 25, 0, '\0', WHITE);
			cout << timers[i];
		}

		for (int k = 0; k <= player.heat; k++)
		{
			drawLine(25 + k, 20, 25 + k, 40, ORANGE);
		}
		for (int k = 0; k <= player.timer; k++)
		{
			drawLine(25 + k, 25, 25 + k, 45, WHITE);
		}
		

		baseM.draw();
		attackM.draw();
		techM.draw();
		itemM.draw();
		tacM.draw();

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