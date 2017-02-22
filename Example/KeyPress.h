#pragma once

#include "Base.h"
#include "Entity.h"
#include "Factory.h"
#include "sfwdraw.h"
#include "UIText.h"
#include <iostream>
using namespace std;
using namespace sfw;
using namespace base;


bool release = true;
bool press = false;

void keyCheck()
{

}

bool getKeyDown(unsigned int keycode)
{
	if (release && !press && getKey(keycode))
	{
		press = true;
		release = false;
		return true;
	}
	else return false;
}

bool getKeyUp(unsigned int keycode)
{
	if (!release && press && !getKey(keycode))
	{
		press = false;
		release = true;
		return true;
	}
}