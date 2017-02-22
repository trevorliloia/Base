#pragma once

#include "Base.h"
#include "Entity.h"
#include "Factory.h"
#include <iostream>
using namespace std;
using namespace sfw;
using namespace base;

class Menu
{
	struct MenuItem
	{
		int option;
		Text item;
	};
public:
	bool isVisible;
	bool isOpen;
	MenuItem menuitems[64];
	int selected = 0;
	int maxItem;
	bool key = false;

	void navigate()
	{
		if (isVisible && isOpen)
		{
			if (getKey('W') && selected > 0 && key == false)
			{
				key = true;
				selected -= 1;

			}
			else if (getKey('S') && selected < maxItem && key == false)
			{
				key = true;
				selected += 1;

			}
			key = false;
		}
	}
};