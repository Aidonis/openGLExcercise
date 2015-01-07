#include "Player.h"

using namespace AiMath;

Player::Player()
{
	//Origin point of player
	origin = Vector2(1024.0f * .5f, 768.0f * .5f);
	//Vertecis offset from origin
	vert1 = Vector2(0, 100);
	vert2 = Vector2(100, -100);
	vert3 = Vector2(-100, -100);
}


Player::~Player()
{
}