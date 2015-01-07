#include "player.h"

using namespace AiMath;

player::player()
{
	origin = Vector2(1024.0f * .5f, 768.0f * .5f);
	vert1 = Vector2(0, 100);
	vert2 = Vector2(100, -100);
	vert3 = Vector2(-100, -100);
}


player::~player()
{
}