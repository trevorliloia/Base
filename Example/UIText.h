#pragma once
#include <cstring>
#include "Base.h"
#include "sfwdraw.h"
#include "BaseState.h"
#include <iostream>

namespace base
{

	class UIText
	{
		char m_characters[80];

	public:
		unsigned sprite_id;
		vec2 offset = vec2{ 0,0 };
		vec2 off_scale = vec2{ 1, 1 };
		void setString(const char *src) { strcpy_s(m_characters, 80, src); }
		UIText() { setString(""); }

		const char operator=(char *a_t)
		{
			setString(a_t);
		}

		void draw(const Transform *T)
		{
			auto glob = T->getGlobalTransform();

			auto pos = glob.getTrans2D() + offset;
			auto dim = glob.getScale2D();

			sfw::drawString(sprite_id, m_characters, pos.x, pos.y, dim.x*off_scale.x, dim.y*off_scale.y, 0);
		}
	};

}