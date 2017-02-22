#pragma once
#include "sfwdraw.h"

namespace kpc
{
	bool oldStates[256];
	bool newStates[256];

	bool getKeyPress(unsigned keycode)
	{
		return newStates[keycode];
	}
	bool getKeyDown(unsigned keycode)
	{
		return !oldStates[keycode] && newStates[keycode];
	}
	bool getKeyUp(unsigned keycode)
	{
		return oldStates[keycode] && !newStates[keycode];
	}

	void poll()
	{
		// move old data to old array
		for (unsigned i = 0; i < 256; ++i)
		{
			oldStates[i] = newStates[i];
		}

		// check new data
		for (unsigned i = 0; i < 256; ++i)
		{
			newStates[i] = sfw::getKey(i);
		}
	}
}