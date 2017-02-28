#pragma once

#include "Base.h"
#include "Entity.h"
#include "Factory.h"
#include "sfwdraw.h"
#include "UIText.h"
#include "Input.h"
#include <iostream>
using namespace std;
using namespace sfw;
using namespace base;

class Menu
{

	struct MenuItem
	{
		int option;
		UIText item;
		bool pick = false;
		int useID;
	};
public:
	Transform menuPos;
	bool isVisible = true;
	bool isOpen = true;
	MenuItem menuitems[64];
	int selected = 0;
	int maxItem;
	bool key = false;
	Menu()
	{

	}
	Menu(Transform pos, int max, UIText items[64], bool visible, bool open, unsigned font)
	{
		menuPos = pos;
		maxItem = max;
		for (int i = 0; i < maxItem; i++)
		{
			menuitems[i].option = i;
			menuitems[i].item = items[i];
			menuitems[i].item.sprite_id = font;
		}
		isVisible = visible;
		isOpen = open;
	}
	Menu(Transform pos, int max, UIText items[64], bool visible, bool open, unsigned font, int id[64])
	{
		menuPos = pos;
		maxItem = max;
		for (int i = 0; i < maxItem; i++)
		{
			menuitems[i].option = i;
			menuitems[i].item = items[i];
			menuitems[i].item.sprite_id = font;
			menuitems[i].useID = id[i];
		}
		isVisible = visible;
		isOpen = open;
	}

	void navigate()
	{
		if (isVisible && isOpen)
		{
			if (kpc::getKeyDown('W') && selected > 0)
			{
				selected -= 1;
			}
			else if (kpc::getKeyDown('S') && selected < (maxItem - 1))
			{
				selected += 1;
			}

			if (kpc::getKeyDown('E'))
			{
				menuitems[selected].pick = true;
			}
		}
	}

	void draw()
	{
		if (isVisible)
		{
			for (int i = 0; i < maxItem; i++)
			{
				Transform itempos = menuPos;
				itempos.setGlobalPosition(vec2{ menuPos.getGlobalPosition().x, menuPos.getGlobalPosition().y - (i * 25) });
				//itempos.setGlobalPosition(vec2{ 0, 0 });
				itempos.setGlobalScale(vec2{ 25,25 });
				menuitems[i].item.draw(&itempos);
				if (selected == i)
				{
					

					drawLine(menuPos.getGlobalPosition().x, menuPos.getGlobalPosition().y - (i * 25), menuPos.getGlobalPosition().x + 30, menuPos.getGlobalPosition().y - (i * 25), RED);
					drawLine(menuPos.getGlobalPosition().x, (menuPos.getGlobalPosition().y - 1) - (i * 25), menuPos.getGlobalPosition().x + 30, menuPos.getGlobalPosition().y - (i * 25), RED);
					drawLine(menuPos.getGlobalPosition().x, (menuPos.getGlobalPosition().y - 2) - (i * 25), menuPos.getGlobalPosition().x + 30, menuPos.getGlobalPosition().y - (i * 25), RED);
					drawLine(menuPos.getGlobalPosition().x, (menuPos.getGlobalPosition().y - 3) - (i * 25), menuPos.getGlobalPosition().x + 30, menuPos.getGlobalPosition().y - (i * 25), RED);
				}
			}
		}
	}
};