#include "Player.h"

Player::Player()
{
	//Origin point of player
	origin = vec2(1024.0f * .5f, 768.0f * .5f);
	//Vertecis offset from origin
	vert1 = vec2(0, 100);
	vert2 = vec2(100, -100);
	vert3 = vec2(-100, -100);
}


Player::~Player()
{
}